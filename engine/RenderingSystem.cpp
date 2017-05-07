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
	if (ev.key == GLFW_KEY_H && ev.action == 0)
	{
		hdr = !hdr;
	}

	if (ev.key == GLFW_KEY_B && ev.action == 0)
	{
		bloom = !bloom;
	}

	if (ev.key == GLFW_KEY_E && ev.action == 0)
	{
		exposure += 0.1f;
	}

	if (ev.key == GLFW_KEY_Q && ev.action == 0)
	{
		exposure -= 0.1f;
	}

	if (ev.key == GLFW_KEY_T && ev.action == 0)
	{
		gamma += 0.1f;
	}

	if (ev.key == GLFW_KEY_R && ev.action == 0)
	{
		gamma -= 0.1f;
	}

	std::cout << "HDR: " << hdr << std::endl;
	std::cout << "Bloom: " << bloom << std::endl;
	std::cout << "Exposure: " << exposure << std::endl;
	std::cout << "Gamma: " << gamma << std::endl;
}

void RenderingSystem::startUp()
{
	std::cout << "Rendering system starting up!" << std::endl;

	ev->addSubscriber<KeyEvent>(this);

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

	ShaderProgram* hdrShader = new ShaderProgram{
	"../res/shaders/hdrVert.shader",
	"../res/shaders/hdrFrag.shader" };

	ShaderProgram* bloomBlurShader = new ShaderProgram{
	"../res/shaders/bloomBlurVert.shader",
	"../res/shaders/bloomBlurFrag.shader" };

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

	try
	{
		hdrShader->compile();
		hdrShader->bindAttribLocation(0, "position");
		hdrShader->bindAttribLocation(1, "texCoords");
		hdrShader->link();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	try
	{
		bloomBlurShader->compile();
		bloomBlurShader->bindAttribLocation(0, "position");
		bloomBlurShader->bindAttribLocation(1, "texCoords");
		bloomBlurShader->link();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	am->registerAsset<ShaderProgram>();

	am->store("simpleShader", program);
	am->store("depthShader", depthShader);
	am->store("hdrShader", hdrShader);
	am->store("bloomBlurShader", bloomBlurShader);

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
				GL_DEPTH_COMPONENT32,
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

	//=========================================================================
	// Setup Post-processing
	//=========================================================================

	//=========================================================================
	// HDR
	//=========================================================================

	glGenFramebuffers(1, &hdrFBO);

	glGenTextures(2, colorBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);


	for (int i = 0; i < 2; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window->getWidth(), window->getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}


	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window->getWidth(), window->getHeight());

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "HDR framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Setup rendering quad

	static GLfloat quadVertices[] = {
		// Positions			// Texture Coords
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//=========================================================================
	// Bloom
	//=========================================================================

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (int i = 0; i < 2; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window->getWidth(), window->getHeight(), 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "PingPong FBO " << i << " not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		shader->uploadUniform(std::string("depthMaps[") + std::to_string(i) + std::string("]"), static_cast<int>(i + 1));
		shader->uploadUniform("textureUnit", 0);
	}

	//=========================================================================
	// Color render pass
	//=========================================================================

	glCullFace(GL_BACK);

	// Restore default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//=========================================================================
	// Bloom render pass
	//=========================================================================

	ShaderProgram* bloomBlurShader = am->fetch<ShaderProgram>("bloomBlurShader");

	bloomBlurShader->use();

	GLboolean horizontal = true;
	GLboolean first_iteration = true;
	
	GLuint amount = 10;

	for(GLuint i = 0; i < amount; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		bloomBlurShader->uploadUniform("horizontal", horizontal);
		bloomBlurShader->uploadUniform("image", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//=========================================================================
	// Screen render pass
	//=========================================================================

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderProgram* hdrShader = am->fetch<ShaderProgram>("hdrShader");

	hdrShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	hdrShader->uploadUniform("hdrBuffer", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	hdrShader->uploadUniform("bloomBuffer", 1);
	hdrShader->uploadUniform("hdr", hdr);
	hdrShader->uploadUniform("bloom", bloom);
	hdrShader->uploadUniform("exposure", exposure);
	hdrShader->uploadUniform("gamma", gamma);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
