#pragma once
#include "Component.h"
#include "EngineDLL.h"
#include <rapidxml/rapidxml.hpp>
#include <cstdint>

class ENGINE_API QuadtreeComponent :
	public Component
{
public:
	QuadtreeComponent() = default;
	~QuadtreeComponent() = default;
	explicit QuadtreeComponent(rapidxml::xml_node<>* node) : position{0} {}

	uint32_t getPosition() const { return position; }
	void setPosition(uint32_t val) { position = val; }
private:
	uint32_t position{0};
};

