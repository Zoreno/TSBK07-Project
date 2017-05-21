#pragma once
#include "Event.h"
#include "EntityManager.h"

typedef uint32_t EntityHandle;

/**
 * \brief Component assignment event
 * \tparam T Type of component that got assigned
 */
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

	/**
	 * \brief Handle to entity that was assigned the component
	 */
	EntityHandle entHandle;
};

