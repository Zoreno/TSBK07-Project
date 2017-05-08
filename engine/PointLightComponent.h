#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <rapidxml/rapidxml.hpp>

class PointLightComponent : public Component
{
public:
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

	explicit PointLightComponent(rapidxml::xml_node<>* node) {} // TODO: Implement

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
private:
};
