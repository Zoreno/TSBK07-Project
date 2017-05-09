/**
 * @file	TextureComponent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Texture Component
 */

#pragma once

#include "Component.h"
#include <GL/glew.h>
#include <map>
#include <rapidxml/rapidxml.hpp>

/**
 * @brief Texture Component
 */
class TextureComponent : public Component
{
public:

	/**
	 * @brief Constructor
	 */
	TextureComponent();

	/**
	 * @brief XML constructor.
	 * @param node XML node.
	 */
	explicit TextureComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	/**
	 * @brief Attach a texture to the component.
	 * @param texUnit Texture Unit
	 * @param ID ID of texture.
	 */
	void attach(GLuint texUnit, const std::string& ID);

	/**
	 * @brief Detach a texture
	 * @param texUnit TextureUnit.
	 */
	void detach(GLuint texUnit);

	/**
	 * @brief Map of textures.
	 */
	std::map<GLuint, std::string> textureMap;
};
