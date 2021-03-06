/**
 * @file	CameraController.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Camera controller
 */

#include "CameraController.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

#include "EntityManager.h"
#include "CollisionComponent.h"
#include "UILabel.h"
#include "PointLightComponent.h"

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
		case GLFW_KEY_SPACE:
			space_pressed = true;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			shift_pressed = true;
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
		case GLFW_KEY_SPACE:
			space_pressed = false;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			shift_pressed = false;
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

	currentCamera = em->createEntity();

	em->assignComponent<TransformComponent>(currentCamera, glm::vec3{ 92.f, 20.f, 170.f });
	em->assignComponent<CameraComponent>(currentCamera);
	//em->assignComponent<CollisionComponent>(currentCamera, 1.f);

	em->getComponent<CameraComponent>(currentCamera)->camera.setPosition(em->getComponent<TransformComponent>(currentCamera)->position);

	std::cout << "Camera handle: " << currentCamera << std::endl;
}

void CameraController::shutDown()
{
	ev->removeSubscriber<KeyEvent>(this);
	ev->removeSubscriber<MouseEvent>(this);

	em->destroyEntity(currentCamera);
}

void CameraController::update(float dt)
{
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
	if (space_pressed)
		ca->camera.processKeyboard(CameraMovement::ABSUP, dt);
	if (shift_pressed)
		ca->camera.processKeyboard(CameraMovement::ABSDOWN, dt);
	GLfloat xOffset = mousePosX - lastMousePosX;
	GLfloat yOffset = lastMousePosY - mousePosY;

	ca->camera.processMouseMovement(xOffset, yOffset);

	lastMousePosX = mousePosX;
	lastMousePosY = mousePosY;

	tr->position = ca->camera.getPosition();

	std::string s;
	std::stringstream ss;

	ss << tr->position;

	s = ss.str();

	ui->getElement<userinterface::UILabel>("coords")->setText(s);
}
