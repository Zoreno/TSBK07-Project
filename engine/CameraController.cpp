#include "CameraController.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "EntityManager.h"

void CameraController::handleEvent(const KeyEvent& ev)
{
	switch (ev.action)
	{
	case 0: // PRESS
		switch (ev.key)
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

void CameraController::handleEvent(const MouseEvent& ev)
{
	mousePosX = ev.posX;
	mousePosY = ev.posY;

	if (firstMouse)
	{
		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;
		firstMouse = false;
	}
}

void CameraController::startUp()
{
	ev->addSubscriber<KeyEvent>(this);
	ev->addSubscriber<MouseEvent>(this);
}

void CameraController::shutDown()
{
	ev->removeSubscriber<KeyEvent>(this);
	ev->removeSubscriber<MouseEvent>(this);
}

void CameraController::update(float dt)
{
	auto findCamera = [&](EntityHandle entHandle, TransformComponent* tr, CameraComponent* ca)
	{
		currentCamera = entHandle;
	};

	if(currentCamera == INVALID_ENTITY)
	{
		em->each<TransformComponent, CameraComponent>(findCamera);
	}

	if(currentCamera == INVALID_ENTITY)
	{
		return;
	}

	TransformComponent* tr = em->getComponent<TransformComponent>(currentCamera);
	CameraComponent* ca = em->getComponent<CameraComponent>(currentCamera);

	if (w_pressed)
		ca->camera.processKeyboard(CameraMovement::FORWARD, dt);
	if (a_pressed)
		ca->camera.processKeyboard(CameraMovement::LEFT, dt);
	if (s_pressed)
		ca->camera.processKeyboard(CameraMovement::BACKWARD, dt);
	if (d_pressed)
		ca->camera.processKeyboard(CameraMovement::RIGHT, dt);

	GLfloat xOffset = mousePosX - lastMousePosX;
	GLfloat yOffset = lastMousePosY - mousePosY;

	ca->camera.processMouseMovement(xOffset, yOffset);

	lastMousePosX = mousePosX;
	lastMousePosY = mousePosY;

	tr->position = ca->camera.getPosition();
}
