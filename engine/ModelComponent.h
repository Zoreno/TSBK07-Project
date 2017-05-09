/**
 * @file	ModelComponent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Model Component
 */

#pragma once

#include "Component.h"

#include <string>
#include <rapidxml/rapidxml.hpp>

/**
 * @brief Model component
 */
class ModelComponent : public Component
{
public:
	/**
	 * @brief Constructor
	 * @param ID ID of model
	 */
	explicit ModelComponent(const std::string& ID) : ID{ID} {}
	
	/**
	 * @brief XML Constructor
	 * @param node XML node
	 */
	explicit ModelComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	/**
	 * @brief ID getter
	 * @return ID of model.
	 */
	const std::string& getID() const { return ID; }

private:

	/**
	 * @brief Model ID.
	 */
	std::string ID{};
};
