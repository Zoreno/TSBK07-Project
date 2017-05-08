#pragma once
#include "Component.h"

#include "TerrainModel.h"
#include <rapidxml/rapidxml.hpp>

class ENGINE_API TerrainComponent : public Component
{
public:
	explicit TerrainComponent(const std::string& ID) : ID{ ID } {}
	explicit TerrainComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	const std::string& getID() const { return ID; }

private:
	std::string ID;
};
