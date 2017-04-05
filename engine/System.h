/**
 * @file	System.h
 * @Author	Joakim Bertils
 * @date	2017-04-05
 * @brief	System Base Class
 */

#pragma once

class EntityManager;
class EventManager;

/**
 * @brief ID unique to each entity.
 */
typedef uint32_t EntityHandle;

/**
 * @brief System Base Class.
 */
class System
{
public:
	/**
	 * @brief Destructors
	 */
	virtual ~System() {}

	/**
	 * @brief Registers managers in the default way. Should not be overridden without good reason.
	 * @param em Pointer to Entity Manager.
	 * @param ev Pointer to Event Manager
	 */
	virtual void registerManagers(EntityManager* em, EventManager* ev) { this->em = em; this->ev = ev; }

	/**
	 * @brief Startup Function. Called before system goes live.
	 */
	virtual void startUp() {}

	/**
	 * @brief Shutdown Function. Called after system has been discontinued.
	 */
	virtual void shutDown() {}

	/**
	 * @brief Update Function.
	 * @param dt Step time in seconds.
	 */
	virtual void update(float dt) = 0;
protected:

	/**
	 * @brief Pointer to Entity Manager.
	 */
	EntityManager* em{ nullptr };

	/**
	 * @brief Pointer to Event Manager.
	 */
	EventManager* ev{ nullptr };
};