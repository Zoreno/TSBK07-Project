/**
* @file	ProjectileComponent.h
* @Author	Kevin Kjellén
* @date	    2017-05-12
* @brief	Projectile component
*/

#pragma once
#include "Component.h"
#include <rapidxml/rapidxml.hpp>
#include <glm/detail/type_vec3.hpp>

/**
 * \brief Projectile component
 */
class ProjectileComponent :
	public Component
{
public:
	ProjectileComponent() = delete;
	/**
	 * \brief Constructor
	 * \param speed Speed of projectile (coords/sec)
	 * \param direction Direction vector of projectile
	 */
	explicit ProjectileComponent(float speed, glm::vec3 direction) : _direction{ direction }, _speed{ speed }, _duration{0} {};

	/**
	 * \brief Constructor from xml
	 * \param node xml node 
	 */
	explicit ProjectileComponent(rapidxml::xml_node<>* node) : _direction{}, _speed{}, _duration{ 0 } {}
	~ProjectileComponent() = default;

	/**
	 * \brief Direction vector 
	 */
	glm::vec3 _direction{};

	/**
	 * \brief Speed of projectile (coords/sec)
	 */
	float _speed{};

	/**
	 * \brief Duration the projectile has been alive
	 */
	float _duration{};
};

