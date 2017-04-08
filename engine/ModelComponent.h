#pragma once

#include "Component.h"

#include <string>
#include <rapidxml/rapidxml.hpp>

class ModelComponent : public Component
{
public:
	explicit ModelComponent(const std::string& ID) : ID{ID} {}
	explicit ModelComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	const std::string& getID() const { return ID; }

private:
	std::string ID{};
};
