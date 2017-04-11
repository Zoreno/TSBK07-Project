#include "RenderingSystem.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include "ShaderProgram.h"

#include <iostream>

#include "EntityManager.h"

#include <GLFW/glfw3.h>
#include "ModelComponent.h"

RenderingSystem::RenderingSystem(Window* window)
	: window{window} {}

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
}

void RenderingSystem::update(float dt)
{
	glm::mat4 proj = glm::perspective(FOV, window->getAspectRatio(), NEAR_PLANE, FAR_PLANE);
	glm::mat4 view;

	auto updateCamera = [&](EntityHandle entHandle, TransformComponent* tr, CameraComponent* ca)
	{
		view = ca->camera.getViewMatrix();
	};

	em->each<TransformComponent, CameraComponent>(updateCamera);

	ShaderProgram* shader = am->fetch<ShaderProgram>("simpleShader");

	auto renderAll = [&](EntityHandle entHandle, TransformComponent* tr, ModelComponent* mc)
	{
		TransformPipeline3D pipe = tr->getPipeline();

		pipe.setProj(proj);
		pipe.setView(view);

		shader->uploadUniform("transform", pipe.getMVP());
		shader->uploadUniform("model", pipe.getModelTransform());

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		am->fetch<RawModel>(mc->getID())->draw();
	};

	em->each<TransformComponent, ModelComponent>(renderAll);
}
