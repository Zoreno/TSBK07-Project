#pragma once
#include "System.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Subscriber.h"
#include "EntityManager.h"

class ENGINE_API CameraController : public System, public Subscriber<KeyEvent>, public Subscriber<MouseEvent>
{
public:

	CameraController() {}

	void handleEvent(const KeyEvent& ev) override;
	void handleEvent(const MouseEvent& ev) override;

	void startUp() override;

	void shutDown() override;

	void update(float dt) override;

	bool w_pressed{ false };
	bool a_pressed{ false };
	bool s_pressed{ false };
	bool d_pressed{ false };

	GLfloat mousePosX{ 0.f };
	GLfloat mousePosY{ 0.f };
	GLfloat lastMousePosX{ 0.f };
	GLfloat lastMousePosY{ 0.f };

	bool firstMouse{ true };

private:
	EntityHandle currentCamera{INVALID_ENTITY};
};
