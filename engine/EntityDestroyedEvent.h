#pragma once

#include "Event.h"
#include "EntityManager.h"

/**
* @brief Event thrown by EventManager when an entity is destroyed.
*/
class ENGINE_API EntityDestroyedEvent : public Event
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