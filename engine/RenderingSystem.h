#pragma once

#include "System.h"
#include "Subscriber.h"
#include "KeyEvent.h"
#include "Camera.h"
#include "Window.h"

class RenderingSystem : public System, public Subscriber<KeyEvent>
{
public:

	explicit RenderingSystem(Window* window);

	void handleEvent(const KeyEvent& ev) override;

	void startUp() override;

	void shutDown() override;

	void update(float dt) override;

	Window* window;
	const GLfloat FOV{ glm::radians(45.f) };
	const GLfloat NEAR_PLANE{ 0.1f };
	const GLfloat FAR_PLANE{ 100.f };
};
