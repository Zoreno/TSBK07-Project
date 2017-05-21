/**
* @file	EntityDestroyedEvent.h
* @Author	Joakim Bertils
* @date	    2017-05-12
* @brief	Event for when an enitiy was destroyed
*/

#pragma once

#include "Event.h"
#include "EntityManager.h"

/**
* @brief Event thrown by EventManager when an entity is destroyed.
*/
class EntityDestroyedEvent : public Event
{
public:
	/**
	* @brief Constructor
	*/
	EntityDestroyedEvent() = default;

	/**
	* @brief Destructor.
	*/
	~EntityDestroyedEvent() override {}

	/**
	* @brief Constructor.
	* @param entHandle Entity Handle.
	*/
	explicit EntityDestroyedEvent(EntityHandle entHandle) : entHandle(entHandle) {}

	/**
	* @brief Entity Handle.
	*/
	EntityHandle entHandle;
};