#pragma once

#include "System.h"
#include "Subscriber.h"
#include "KeyEvent.h"
#include "Camera.h"

class RenderingSystem : public System, public Subscriber<KeyEvent>
{
public:

	RenderingSystem();

	void handleEvent(const KeyEvent& ev) override;

	void startUp() override;

	void shutDown() override;

	void update(float dt) override;

	bool w_pressed{ false };
	bool a_pressed{ false };
	bool s_pressed{ false };
	bool d_pressed{ false };
};
