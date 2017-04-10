#pragma once

#include "System.h"
#include "Subscriber.h"
#include "KeyEvent.h"
#include "Camera.h"
#include "Window.h"

class RenderingSystem : public System, public Subscriber<KeyEvent>
{
public:

	RenderingSystem(Window* window);

	void handleEvent(const KeyEvent& ev) override;

	void startUp() override;

	void shutDown() override;

	void update(float dt) override;

	bool w_pressed{ false };
	bool a_pressed{ false };
	bool s_pressed{ false };
	bool d_pressed{ false };

	Window* window;
	const GLfloat FOV{ glm::radians(45.f) };
	const GLfloat NEAR_PLANE{ 0.1f };
	const GLfloat FAR_PLANE{ 100.f };
};
