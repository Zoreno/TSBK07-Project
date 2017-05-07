#include "RenderingSystem.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include "ShaderProgram.h"

#include <iostream>

#include "EntityManager.h"

#include <GLFW/glfw3.h>
#include "ModelComponent.h"
#include "TerrainComponent.h"
#include "TextureComponent.h"
#include "PointLight.h"
#include "PointLightComponent.h"
#include "MaterialComponent.h"

RenderingSystem::RenderingSystem(Window* window)
	: window{ window } {}

void RenderingSystem::handleEvent(const KeyEvent& ev)
{

}

void RenderingSystem::startUp()
{
	std::cout << "Rendering system starting up!" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderProgram* program = new ShaderProgram{
		"../res/shaders/simpleVert.shader",
		"../res/shaders/simpleFrag.shader" };
	ShaderProgram* depthShader = new ShaderProgram{
		"../res/shaders/depthVert.shader",
		"../res/shaders/depthFrag.shader" ,
		"../res/shaders/depthGeom.shader" };

	try
	{
		program->compile();
		program->bindAttribLocation(0, "vertex_position");
		program->bindAttribLocation(1, "vertex_normal");
		program->bindAttribLocation(2, "vertex_texture_coordinates");
		program->link();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	try
	{
		depthShader->compile();
		depthShader->bindAttribLocation(0, "vertex_position");
		depthShader->link();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	am->registerAsset<ShaderProgram>();

	am->store("simpleShader", program);
	am->store("depthShader", depthShader);

	//=========================================================================
	// Setup Depth Rendering
	//=========================================================================

	// Generate Framebuffer to render depth data
	glGenFramebuffers(MAX_LIGHTS, depthMapFBOs);

	// Generate our depth map slots
	glGenTextures(MAX_LIGHTS, depthMaps);

	GLfloat borderColor[] = { 1.f, 1.f, 1.f, 1.f };

	// Initialize all textures
	for (size_t i = 0; i < MAX_LIGHTS; ++i)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMaps[i]);

		// Initialize all 6 faces of the cubemap
		for (size_t face = 0; face < 6; ++face)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,
				GL_DEPTH_COMPONENT,
				SHADOW_WIDTH,
				SHADOW_HEIGHT,
				0,
				GL_DEPTH_COMPONENT,
				GL_FLOAT,
				NULL);
		}

		// Setup sampling behaviour
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Setup wrapping behaviour
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		// Setup border color
		glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

		// Setup framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMaps[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		// Check if setup is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "Framebuffer " << i << " not complete" << std::endl;
		}

		// Unbind buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}

void RenderingSystem::shutDown()
{
	std::cout << "Rendering system shutting down!" << std::endl;
	ev->removeSubscriber<KeyEvent>(this);
	am->dispose<ShaderProgram>("simpleShader");
	am->dispose<ShaderProgram>("depthShader");
}

void RenderingSystem::update(float dt)
{
	glm::mat4 proj = glm::perspective(FOV, window->getAspectRatio(), NEAR_PLANE, FAR_PLANE);

	glm::mat4 view;
	glm::vec3 view_pos;

	// This is hax
	auto updateCamera = [&](EntityHandle entHandle, TransformComponent* tr, CameraComponent* ca)
	{
		view = ca->camera.getViewMatrix();
		view_pos = tr->position;
	};

	em->each<TransformComponent, CameraComponent>(updateCamera);

	ShaderProgram* shader = am->fetch<ShaderProgram>("simpleShader");
	ShaderProgram* depthShader = am->fetch<ShaderProgram>("depthShader");

	shader->uploadUniform("viewPos", view_pos);

	std::vector<PointLight> lights;

	auto getLights = [&](EntityHandle entHandle, TransformComponent* tr, PointLightComponent* pl)
	{
		PointLight pointLight{
		tr->position,
		pl->ambient,
		pl->diffuse,
		pl->specular,
		pl->constant,
		pl->linear,
		pl->quadratic };

		lights.push_back(pointLight);
	};

	em->each<TransformComponent, PointLightComponent>(getLights);

	shader->use();

	for (int i = 0; i < lights.size(); ++i)
	{
		std::string name{ "lights[" };
		name += std::to_string(i);
		name += "]";
		shader->uploadUniform(name, lights[i]);
	}

	shader->uploadUniform("numLights", static_cast<int>(lights.size()));

	//=========================================================================
	// Depth render pass
	//=========================================================================

	static constexpr GLfloat SHADOW_ASPECT = static_cast<GLfloat>(SHADOW_WIDTH) / static_cast<GLfloat>(SHADOW_HEIGHT);

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.f), SHADOW_ASPECT, SHADOW_NEAR_PLANE, SHADOW_FAR_PLANE);

	auto renderTerrainDepth = [&](EntityHandle entHandle, TransformComponent* tr, TerrainComponent* te)
	{
		glm::mat4 model = glm::translate(glm::mat4{ 1.f }, tr->position);
		depthShader->uploadUniform("model", model);

		am->fetch<TerrainModel>(te->getID())->draw(*depthShader);
	};

	auto renderModelsDepth = [&](EntityHandle entHandle, TransformComponent* tr, ModelComponent* mc)
	{
		glm::mat4 model = glm::translate(glm::mat4{ 1.f }, tr->position);
		depthShader->uploadUniform("model", model);

		am->fetch<RawModel>(mc->getID())->draw();
	};

	for (size_t i = 0; i < lights.size(); ++i)
	{
		std::vector<glm::mat4> shadowTransforms;

		PointLight& light = lights[i];

		// Positive x Direction
		shadowTransforms.push_back(
			shadowProj*glm::lookAt(
				light.getPosition(), // Eye
				light.getPosition() + glm::vec3(1.0, 0.0, 0.0), // Center
				glm::vec3(0.0, -1.0, 0.0) // Up
			));

		// Negative x Direction
		shadowTransforms.push_back(
			shadowProj*glm::lookAt(
				light.getPosition(), // Eye
				light.getPosition() + glm::vec3(-1.0, 0.0, 0.0), // Center
				glm::vec3(0.0, -1.0, 0.0) // Up
			));

		// Positive y Direction
		shadowTransforms.push_back(
			shadowProj*glm::lookAt(
				light.getPosition(), // Eye
				light.getPosition() + glm::vec3(0.0, 1.0, 0.0), // Center
				glm::vec3(0.0, 0.0, 1.0) // Up
			));

		// Negative y Direction
		shadowTransforms.push_back(
			shadowProj*glm::lookAt(
				light.getPosition(), // Eye
				light.getPosition() + glm::vec3(0.0, -1.0, 0.0), // Center
				glm::vec3(0.0, 0.0, -1.0) // Up
			));

		// Positive z Direction
		shadowTransforms.push_back(
			shadowProj*glm::lookAt(
				light.getPosition(), // Eye
				light.getPosition() + glm::vec3(0.0, 0.0, 1.0), // Center
				glm::vec3(0.0, -1.0, 0.0) // Up
			));

		// Negative z Direction
		shadowTransforms.push_back(
			shadowProj*glm::lookAt(
				light.getPosition(), // Eye
				light.getPosition() + glm::vec3(0.0, 0.0, -1.0), // Center
				glm::vec3(0.0, -1.0, 0.0) // Up
			));

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBOs[i]);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);

		// Upload shadow transforms
		for (int j = 0; j < 6; ++j)
		{
			depthShader->uploadUniform(std::string("shadowMatrices[") + std::to_string(j) + std::string("]"), shadowTransforms[j]);
		}

		depthShader->uploadUniform("far_plane", SHADOW_FAR_PLANE);
		shader->uploadUniform("far_plane", SHADOW_FAR_PLANE); 
		depthShader->uploadUniform("lightPos", light.getPosition());

		em->each<TransformComponent, TerrainComponent>(renderTerrainDepth);
		em->each<TransformComponent, ModelComponent>(renderModelsDepth);

		// Bind texture for next render pass
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMaps[i]);

		// Depth map for light i will be in texture unit i + 1
		shader->uploadUniform(std::string("depthMaps[") + std::to_string(i) + std::string("]"), static_cast<int>(i+1));
		shader->uploadUniform("textureUnit", 0);
	}

	//=========================================================================
	// Color render pass
	//=========================================================================

	glCullFace(GL_BACK);

	// Restore default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, window->getWidth(), window->getHeight());

	auto renderTerrain = [&](EntityHandle entHandle, TransformComponent* tr, TerrainComponent* te)
	{
		shader->use();
		TransformPipeline3D pipe = tr->getPipeline();

		pipe.setProj(proj);
		pipe.setView(view);

		shader->uploadUniform("transform", pipe.getMVP());
		shader->uploadUniform("model", pipe.getModelTransform());

		TextureComponent* tex = em->getComponent<TextureComponent>(entHandle);

		if (tex)
		{
			for (auto it : tex->textureMap)
			{
				//am->fetch<Texture2D>(it.second)->bind(it.first);
				am->fetch<Texture2D>(it.second)->bind(0);
			}
		}

		MaterialComponent* mat = em->getComponent<MaterialComponent>(entHandle);

		if (mat)
		{
			shader->uploadUniform("material", mat->material);
		}
		else
		{
			shader->uploadUniform("material", defaultMaterial);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		am->fetch<TerrainModel>(te->getID())->draw(*shader);
	};

	auto renderModels = [&](EntityHandle entHandle, TransformComponent* tr, ModelComponent* mc)
	{
		shader->use();
		TransformPipeline3D pipe = tr->getPipeline();

		pipe.setProj(proj);
		pipe.setView(view);

		shader->uploadUniform("transform", pipe.getMVP());
		shader->uploadUniform("model", pipe.getModelTransform());

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TextureComponent* tex = em->getComponent<TextureComponent>(entHandle);

		if (tex)
		{
			for (auto it : tex->textureMap)
			{
				am->fetch<Texture2D>(it.second)->bind(it.first);
			}
		}

		MaterialComponent* mat = em->getComponent<MaterialComponent>(entHandle);

		if (mat)
		{
			shader->uploadUniform("material", mat->material);
		}
		else
		{
			shader->uploadUniform("material", defaultMaterial);
		}

		am->fetch<RawModel>(mc->getID())->draw();
	};

	em->each<TransformComponent, TerrainComponent>(renderTerrain);
	em->each<TransformComponent, ModelComponent>(renderModels);
}
