/**
 * @file	CameraComponent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Camera component class
 */

#pragma once

#include "Component.h"

#include "Camera.h"
#include <rapidxml/rapidxml.hpp>

/**
 * @brief Camera component
 */
class CameraComponent : public Component
{
public:
	/**
	 * @brief Default constructor
	 */
	CameraComponent() = default;

	/**
	 * @brief XML constructor
	 * @param node Node with camera info.
	 */
	explicit CameraComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	/**
	 * @brief Camera object
	 */
	Camera camera{};
};