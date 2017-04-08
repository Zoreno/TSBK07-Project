#include "RenderingSystem.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include <iostream>

#include "EntityManager.h"

#include <GLFW/glfw3.h>

RenderingSystem::RenderingSystem()
{
}

void RenderingSystem::handleEvent(const KeyEvent& ev)
{
	std::cout << ev.action << std::endl;

	switch(ev.action)
	{
	case 0: // PRESS
		switch(ev.key)
		{
		case GLFW_KEY_W:
			w_pressed = true;
			break;
		case GLFW_KEY_A:
			a_pressed = true;
			break;
		case GLFW_KEY_S:
			s_pressed = true;
			break;
		case GLFW_KEY_D:
			d_pressed = true;
			break;
		default:
			break;
		}
		break;
	case 2: // RELEASE
		switch (ev.key)
		{
		case GLFW_KEY_W:
			w_pressed = false;
			break;
		case GLFW_KEY_A:
			a_pressed = false;
			break;
		case GLFW_KEY_S:
			s_pressed = false;
			break;
		case GLFW_KEY_D:
			d_pressed = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void RenderingSystem::startUp()
{
	std::cout << "Rendering system starting up!" << std::endl;
	ev->addSubscriber<KeyEvent>(this);
}

void RenderingSystem::shutDown()
{
	std::cout << "Rendering system shutting down!" << std::endl;
	ev->removeSubscriber<KeyEvent>(this);
}

void RenderingSystem::update(float dt)
{
	auto updateCamera = [this, dt](EntityHandle entHandle, TransformComponent* tr, CameraComponent* ca)
	{
		if (w_pressed)
			ca->camera.processKeyboard(CameraMovement::FORWARD, dt);
		if (a_pressed)
			ca->camera.processKeyboard(CameraMovement::LEFT, dt);
		if (s_pressed)
			ca->camera.processKeyboard(CameraMovement::BACKWARD, dt);
		if (d_pressed)
			ca->camera.processKeyboard(CameraMovement::RIGHT, dt);

		tr->position = ca->camera.getPosition();
	};

	em->each<TransformComponent, CameraComponent>(updateCamera);
}
