#pragma once
#include "Component.h"

class WaterComponent : public Component
{
public:
	WaterComponent();

	explicit WaterComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till
};

inline WaterComponent::WaterComponent()
{
	
}
