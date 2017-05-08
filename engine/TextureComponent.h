#pragma once
#include "Component.h"
#include <GL/glew.h>
#include <map>
#include <rapidxml/rapidxml.hpp>

class TextureComponent : public Component
{
public:
	TextureComponent();

	explicit TextureComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	void attach(GLuint texUnit, const std::string& ID);
	void detach(GLuint texUnit);

	std::map<GLuint, std::string> textureMap;
};
