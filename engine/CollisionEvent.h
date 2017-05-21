/**
* @file	CollisionEvent.h
* @Author	Kevin Kjellén
* @date	    2017-05-12
* @brief	Collision event
*/

#pragma once
#include "Event.h"
#include "EntityManager.h"

/**
 * \brief Collision event
 */
class CollisionEvent :
	public Event
{
public:
	/**
	* @brief Constructor.
	* @param entHandle1 Entity Handle for a member of the collision.
	* @param entHandle2 Entity Handle for other member of collision.
	*/
	explicit CollisionEvent(EntityHandle entHandle1, EntityHandle entHandle2) : entHandle1(entHandle1), entHandle2(entHandle2) {}

	~CollisionEvent() = default;

	/**
	 * \brief Handle to entity in the collision
	 */
	EntityHandle entHandle1;

	/**
	 * \brief Handle to entity in the collision
	 */
	EntityHandle entHandle2;
};

