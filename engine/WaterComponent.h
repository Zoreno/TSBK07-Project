#pragma once
#include "Component.h"

class WaterComponent : public Component
{
public:
	WaterComponent();

	explicit WaterComponent(rapidxml::xml_node<>* node) {} // TODO: L�gg till
};

inline WaterComponent::WaterComponent()
{
	
}
