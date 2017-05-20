#pragma once
#include "Component.h"
#include <rapidxml/rapidxml.hpp>
#include <glm/detail/type_vec3.hpp>

class ProjectileComponent :
	public Component
{
public:
	ProjectileComponent() = delete;
	explicit ProjectileComponent(float speed, glm::vec3 direction) : _direction{ direction }, _speed{ speed }, _duration{0} {};
	explicit ProjectileComponent(rapidxml::xml_node<>* node) : _direction{}, _speed{}, _duration{ 0 } {}
	~ProjectileComponent() = default;

	glm::vec3 _direction{};
	float _speed{};
	float _duration{};
};

