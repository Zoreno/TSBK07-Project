/**
 * @file	RenderingSystem.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	System for rendering stuff to screen
 */

#pragma once

#include "System.h"
#include "Subscriber.h"
#include "KeyEvent.h"
#include "Camera.h"
#include "Window.h"

#define MAX_LIGHTS 8

/**
 * @brief System for rendering stuff to screen
 */
class RenderingSystem : public System, public Subscriber<KeyEvent>
{
public:

	/**
	 * @brief Constructor
	 * @param window Ptr to window the scene should be rendered to.
	 */
	explicit RenderingSystem(Window* window);

	/**
	 * @brief Key Event Handler
	 * @param ev Key Event
	 */
	virtual void handleEvent(const KeyEvent& ev) override;

	/**
	 * @brief System Startup routine
	 */
	virtual void startUp() override;

	/**
	 * @brief System Shutdown routine
	 */
	virtual void shutDown() override;

	/**
	 * @brief Update the system, accounting for time dt
	 * @param dt Time delta
	 */
	virtual void update(float dt) override;

	/**
	 * @brief Pointer to window.
	 */
	Window* window;

	/**
	 * @brief Field of View
	 */
	const GLfloat FOV{ glm::radians(45.f) };

	/**
	 * @brief Near Plane of projection
	 */
	const GLfloat NEAR_PLANE{ 0.1f };
	
	/**
	 * @brief Far Plane of projection
	 */
	const GLfloat FAR_PLANE{ 250.f };

private:

	/**
	 * @brief Default material, which will be used if MaterialComponent not is present.
	 */
	Material defaultMaterial{ 
		glm::vec3{1.f,1.f,1.f}, // Ambient
		glm::vec3{1.f,1.f,1.f}, // Diffuse
		glm::vec3{1.f,1.f,1.f}, // Specular
		64						// Shininess 
	};

	/**
	 * @brief Width of shadow texture
	 */
	static constexpr GLuint SHADOW_WIDTH{ 512 };

	/**
	 * @brief Height of shadow texture
	 */
	static constexpr GLuint SHADOW_HEIGHT{ 512 };

	/**
	 * @brief Shadow projection near plane
	 */
	static constexpr GLfloat SHADOW_NEAR_PLANE{ 1.f };

	/**
	 * @brief Shadow projection far plane
	 */
	static constexpr GLfloat SHADOW_FAR_PLANE{ 250.f };

	/**
	 * @brief Light depth frame buffer objects
	 */
	GLuint depthMapFBOs[MAX_LIGHTS];

	/**
	 * @brief Light depth texture cube maps
	 */
	GLuint depthMaps[MAX_LIGHTS];

	/**
	 * @brief HDR framebuffer object
	 */
	GLuint hdrFBO{};

	/**
	 * @brief Renderbuffer for depth
	 */
	GLuint rboDepth{};

	/**
	 * @brief HDR color buffers
	 */
	GLuint colorBuffers[2];

	/**
	 * @brief Vertex array object for screen rendering quad.
	 */
	GLuint quadVAO{};

	/**
	 * @brief Vertex buffer object for screen rendering quad.
	 */
	GLuint quadVBO{};

	/**
	 * @brief PingPong framebuffer for multipass gaussian blur.
	 */
	GLuint pingpongFBO[2];

	/**
	 * @brief PingPong color buffer for multipass gaussian blur.
	 */
	GLuint pingpongColorbuffers[2];

	/**
	 * @brief Whether to use HDR
	 */
	bool hdr{ true };

	/**
	 * @brief Whether to use bloom.
	 */
	bool bloom{ true };

	/**
	 * @brief HDR exposure parameter
	 */
	float exposure{ 1.f };

	/**
	 * @brief HDR gamma parameter
	 */
	float gamma{ 2.2f };
};
