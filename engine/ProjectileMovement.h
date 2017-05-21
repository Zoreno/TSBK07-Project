#pragma once
#include "System.h"
#include "Subscriber.h"
#include "CollisionEvent.h"
#include "EntityManager.h"
#include "KeyEvent.h"
#include "CameraController.h"

class MouseEvent;

/**
 * \brief Projectile movement system
 */
class ProjectileMovement : public System, public Subscriber<KeyEvent>, public Subscriber<CollisionEvent>
{
	friend class CameraController;
public:
	/**
	* @brief Constructor
	*/
	ProjectileMovement() {}

	/**
	* @brief Key Event Handler
	* @param ev Mouse Event
	*/
	void handleEvent(const KeyEvent& ev) override;

	/**
	* @brief Collision Event Handler
	* @param ev Collision Event
	*/
	void handleEvent(const CollisionEvent& ev) override;

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
};

