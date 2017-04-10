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

	ShaderProgram program{ "../res/shaders/simpleVert.shader", "../res/shaders/simpleFrag.shader" };

	try
	{
		program.compile();
		program.bindAttribLocation(0, "vertex_position");
		program.bindAttribLocation(1, "vertex_normal");
		program.bindAttribLocation(2, "vertex_texture_coordinates");

		program.link();
		program.use();
	}
	catch(const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto renderAll = [&](EntityHandle entHandle, TransformComponent* tr, ModelComponent* mc)
	{
		TransformPipeline3D pipe = tr->getPipeline();

		pipe.setProj(proj);
		pipe.setView(view);

		program.uploadUniform("transform", pipe.getMVP());
		program.uploadUniform("model", pipe.getModelTransform());

		am->fetch<RawModel>(mc->getID())->draw();
	};

	em->each<TransformComponent, ModelComponent>(renderAll);
}
