#pragma once
#include "Event.h"
#include "EntityManager.h"

typedef uint32_t EntityHandle;

template <typename T>
class ComponentAssignedEvent : public Event
{
public:
	/**
	* @brief Constructor.
	* @param entHandle Entity Handle.
	*/
	explicit ComponentAssignedEvent(EntityHandle entHandle) : entHandle(entHandle) {}

	~ComponentAssignedEvent() = default;

	EntityHandle entHandle;
};

