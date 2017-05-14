/**
 * @file	CameraController.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Camera controller
 */

#pragma once

#include "System.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "Subscriber.h"
#include "EntityManager.h"

/**
 * @brief Camera controller class
 */
class CameraController : public System, public Subscriber<KeyEvent>, public Subscriber<MouseEvent>
{
public:

	/**
	 * @brief Constructor
	 */
	CameraController() {}

	/**
	 * @brief Key Event Handler
	 * @param ev Key Event
	 */
	void handleEvent(const KeyEvent& ev) override;

	/**
	 * @brief Mouse Event Handler
	 * @param ev Mouse Event
	 */
	void handleEvent(const MouseEvent& ev) override;

	/**
	 * @brief Startup routine
	 */
	void startUp() override;

	/**
	 * @brief Shutdown routine
	 */
	void shutDown() override;

	/**
	 * @brief Update the controller
	 * @param dt Timestep
	 */
	void update(float dt) override;

	/**
	 * @brief Flag to indicate w key status
	 */
	bool w_pressed{ false };

	/**
	* @brief Flag to indicate a key status
	*/
	bool a_pressed{ false };

	/**
	* @brief Flag to indicate s key status
	*/
	bool s_pressed{ false };

	/**
	* @brief Flag to indicate d key status
	*/
	bool d_pressed{ false };

	/**
	* @brief Flag to indicate space key status
	*/
	bool space_pressed{ false };

	/**
	* @brief Flag to indicate shift key status
	*/
	bool shift_pressed{ false };

	/**
	 * @brief Current mouse x-position
	 */
	GLfloat mousePosX{ 0.f };

	/**
	 * @brief Current mouse y-position
	 */
	GLfloat mousePosY{ 0.f };

	/**
	 * @brief Last mouse x-position
	 */
	GLfloat lastMousePosX{ 0.f };

	/**
	 * @brief Last mouse y-position
	 */
	GLfloat lastMousePosY{ 0.f };

	/**
	 * @brief If this is the first time processing the mouse.
	 */
	bool firstMouse{ true };

private:

	/**
	 * @brief Handle of camera entity.
	 */
	EntityHandle currentCamera{INVALID_ENTITY};
};
