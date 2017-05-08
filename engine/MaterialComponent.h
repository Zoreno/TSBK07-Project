#pragma once

#include "Component.h"
#include "Material.h"

#include <rapidxml/rapidxml.hpp>

class MaterialComponent : public Component
{
public:
	MaterialComponent(
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		float shininess)
		: material(ambient, diffuse, specular, shininess) {}

	explicit MaterialComponent(rapidxml::xml_node<>* node) {} // TODO: Implement

	Material material{ glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1.f,1.f,1.f }, glm::vec3{ 1.f,1.f,1.f }, 64 };
};
