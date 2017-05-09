/**
 * @file	TerrainComponent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Terrain Component
 */

#pragma once
#include "Component.h"

#include "TerrainModel.h"
#include <rapidxml/rapidxml.hpp>

/**
 * @brief Terrain Component
 */
class TerrainComponent : public Component
{
public:
	/**
	 * @brief Constructor
	 * @param ID ID of terrain model.
	 */
	explicit TerrainComponent(const std::string& ID) : ID{ ID } {}

	/**
	 * @brief XML Constructor
	 * @param node XML node.
	 */
	explicit TerrainComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	/**
	 * @brief ID getter
	 * @return ID of model. 
	 */
	const std::string& getID() const { return ID; }

private:

	/**
	 * @brief ID of terrain model.
	 */
	std::string ID;
};
