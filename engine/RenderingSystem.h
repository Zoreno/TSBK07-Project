#pragma once

#include "System.h"
#include "Subscriber.h"
#include "KeyEvent.h"
#include "Camera.h"
#include "Window.h"

#define MAX_LIGHTS 32

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

private:

	Material defaultMaterial{ glm::vec3{1.f,1.f,1.f}, glm::vec3{1.f,1.f,1.f}, glm::vec3{1.f,1.f,1.f}, 64 };

	static constexpr GLuint SHADOW_WIDTH{ 1024 };
	static constexpr GLuint SHADOW_HEIGHT{ 1024 };
	static constexpr GLfloat SHADOW_NEAR_PLANE{ 1.f };
	static constexpr GLfloat SHADOW_FAR_PLANE{ 250.f };

	GLuint depthMapFBOs[MAX_LIGHTS];
	GLuint depthMaps[MAX_LIGHTS];

	GLuint hdrFBO;
	GLuint rboDepth;
	GLuint colorBuffers[2];
	GLuint quadVAO;
	GLuint quadVBO;

	GLuint pingpongFBO[2];
	GLuint pingpongColorbuffers[2];

	bool hdr{ true };
	bool bloom{ true };
	float exposure{ 1.f };
	float gamma{ 2.2f };
};
