#pragma once

#include "Component.h"

#include "Camera.h"
#include <rapidxml/rapidxml.hpp>

class ENGINE_API CameraComponent : public Component
{
public:
	CameraComponent() = default;

	explicit CameraComponent(rapidxml::xml_node<>* node) {}

	Camera camera{};
};
