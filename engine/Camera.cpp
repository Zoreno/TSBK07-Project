#include "Camera.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
	:position{ position },
	front{ glm::vec3(0.f,0.f,-1.f) },
	worldUp{ up },
	yaw{ yaw },
	pitch{ pitch },
	movementSpeed{ SPEED },
	mouseSensitivity{ SENSITIVITY }
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, GLfloat timeDelta)
{
	GLfloat velocity = movementSpeed * timeDelta;
	switch (direction)
	{
	case FORWARD:
		position += front*velocity;
		break;
	case BACKWARD:
		position -= front*velocity;
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	case ABSUP:
		position += worldUp * velocity;
		break;
	case ABSDOWN:
		position -= worldUp * velocity;
		break;
	}
}

void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.f)
			pitch = 89.f;
		if (pitch < -89.f)
			pitch = -89.f;
	}

	updateCameraVectors();
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getFrontVector() const
{
	return front;
}

glm::vec3 Camera::getUpVector() const
{
	return up;
}

glm::vec3 Camera::getRightVector() const
{
	return right;
}

glm::vec3 Camera::getWorldUpVector() const
{
	return worldUp;
}

GLfloat Camera::getYaw() const
{
	return yaw;
}

GLfloat Camera::getPitch() const
{
	return pitch;
}

GLfloat Camera::getMovementSpeed() const
{
	return movementSpeed;
}

GLfloat Camera::getMouseSensitivity() const
{
	return mouseSensitivity;
}

void Camera::updateCameraVectors()
{
	glm::vec3 Front;
	Front.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
	Front.y = sin(glm::radians(pitch));
	Front.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
	front = glm::normalize(Front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}