/**
* @file	CollisionComponent.h
* @Author	Kevin Kjellén
* @date	    2017-05-12
* @brief	Collision component
*/

#pragma once
#include "Component.h"
#include <rapidxml/rapidxml.hpp>
/**
 * \brief Collision component
 */
class CollisionComponent :
	public Component
{
public:
	CollisionComponent() = delete;

	/**
	 * \brief Contructor (common constructor)
	 * \param reach The reach of the collider
	 */
	explicit CollisionComponent(float reach) : reach{reach}{};
	
	/**
	 * \brief Contructor from xml
	 * \param node an xml node
	 */
	explicit CollisionComponent(rapidxml::xml_node<>* node) : reach{} {}
	~CollisionComponent() = default;

	/**
	 * \brief Returns the reach of the collider
	 * \return Reach of collider
	 */
	float getReach() const;
private:
	/**
	 * \brief Reach of the collider
	 */
	const float reach;
};

