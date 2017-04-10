#pragma once

#include "Event.h"
#include "EntityManager.h"

/**
* @brief Event thrown by EventManager when a new entity is created.
*/
class EntityCreatedEvent : public Event
{
public:
	/**
	* @brief Constructor
	*/
	EntityCreatedEvent() = default;

	/**
	* @brief Destructor
	*/
	~EntityCreatedEvent() override {}

	/**
	* @brief Constructor.
	* @param entHandle Entity Handle.
	*/
	explicit EntityCreatedEvent(EntityHandle entHandle) : entHandle(entHandle) {}

	/**
	* @brief Entity Handle.
	*/
	EntityHandle entHandle{};
};