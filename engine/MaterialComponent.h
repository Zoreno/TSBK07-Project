/**
 * @file	MaterialComponent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Material component
 */

#pragma once

#include "Component.h"
#include "Material.h"

#include <rapidxml/rapidxml.hpp>

/**
 * @brief Material component class
 */
class MaterialComponent : public Component
{
public:
	/**
	 * @brief Constructor
	 * @param ambient Ambient reflection
	 * @param diffuse Diffuse reflection
	 * @param specular Specular reflecion
	 * @param shininess Shininess
	 */
	MaterialComponent(
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		float shininess)
		: material(ambient, diffuse, specular, shininess) {}

	/**
	 * @brief XML constructor
	 * @param node XML node.
	 */
	explicit MaterialComponent(rapidxml::xml_node<>* node) {} // TODO: Implement

	/**
	 * @brief Material object
	 */
	Material material{ glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1.f,1.f,1.f }, 64 };
};
