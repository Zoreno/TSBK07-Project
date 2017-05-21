#pragma once
#include "Component.h"
#include <rapidxml/rapidxml.hpp>
#include <cstdint>

/**
 * \brief Quadtree component
 */
class QuadtreeComponent :
	public Component
{
public:
	QuadtreeComponent() = default;
	~QuadtreeComponent() = default;
	/**
	 * \brief Constructor for xml
	 * \param node xml node
	 */
	explicit QuadtreeComponent(rapidxml::xml_node<>* node) : position{0} {}

	/**
	 * \brief Returns the position in the quadtree
	 * \return position in quadtree 
	 */
	uint32_t getPosition() const { return position; }

	/**
	 * \brief Sets the position in the quadtree to val
	 * \param val Position to set
	 */
	void setPosition(uint32_t val) { position = val; }
private:
	/**
	 * \brief Position in the quadtree
	 */
	uint32_t position{0};
};

