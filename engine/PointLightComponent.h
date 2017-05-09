/**
 * @file	PointLightComponent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Point Light Component
 */

#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <rapidxml/rapidxml.hpp>

/**
 * @brief Point light component
 */
class PointLightComponent : public Component
{
public:

	/**
	 * @brief Constructor
	 * @param ambient Ambient Color
	 * @param diffuse Diffuse Color
	 * @param specular Specular Color
	 * @param constant Constant attenuation factor
	 * @param linear Linear attenuation factor
	 * @param quadratic Quadratic attenuation factor.
	 */
	PointLightComponent(
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		float constant = 1.f,
		float linear = 0.f,
		float quadratic = 0.f)
		:
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		constant(constant),
		linear(linear),
		quadratic(quadratic)
	{}

	/**
	 * @brief XML Constructor
	 * @param node XML node.
	 */
	explicit PointLightComponent(rapidxml::xml_node<>* node) {} // TODO: Implement

	/**
	 * @brief Ambient Color
	 */
	glm::vec3 ambient;

	/**
	 * @brief Diffuse Color.
	 */
	glm::vec3 diffuse;
	
	/**
	 * @brief Specular color.
	 */
	glm::vec3 specular;

	/**
	 * @brief Constant attenuation factor.
	 */
	float constant;
	
	/**
	 * @brief Linear attenuation factor.
	 */
	float linear;
	
	/**
	 * @brief Quadratic attenuation factor.
	 */
	float quadratic;
};
