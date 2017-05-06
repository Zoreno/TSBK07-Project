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

	ShaderProgram* program = new ShaderProgram{ "../res/shaders/simpleVert.shader", "../res/shaders/simpleFrag.shader" };

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

	am->registerAsset<ShaderProgram>();

	am->store("simpleShader", program);
}

void RenderingSystem::shutDown()
{
	std::cout << "Rendering system shutting down!" << std::endl;
	ev->removeSubscriber<KeyEvent>(this);
	am->dispose<ShaderProgram>("simpleShader");
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

	//=========================================================================
	// Color render pass
	//=========================================================================

	shader->use();

	for(int i = 0; i < lights.size(); ++i)
	{
		std::string name{ "lights[" };
		name += std::to_string(i);
		name += "]";
		shader->uploadUniform(name, lights[i]);
	}

	shader->uploadUniform("numLights", static_cast<int>(lights.size()));

	shader->uploadUniform("material.ambient", glm::vec3{ 1.f,1.f,1.f });
	shader->uploadUniform("material.diffuse", glm::vec3{ 1.f,1.f,1.f });
	shader->uploadUniform("material.specular", glm::vec3{ 1.f,1.f,1.f });
	shader->uploadUniform("material.shininess", 64.f);

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
				am->fetch<Texture2D>(it.second)->bind(it.first);
			}
		}

		MaterialComponent* mat = em->getComponent<MaterialComponent>(entHandle);

		if(mat)
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

	auto renderAll = [&](EntityHandle entHandle, TransformComponent* tr, ModelComponent* mc)
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
	em->each<TransformComponent, ModelComponent>(renderAll);
}
