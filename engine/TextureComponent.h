#pragma once
#include "Component.h"
#include "EngineDLL.h"
#include <GL/glew.h>
#include <map>
#include <rapidxml/rapidxml.hpp>

class ENGINE_API TextureComponent : public Component
{
public:
	TextureComponent();

	explicit TextureComponent(rapidxml::xml_node<>* node) {} // TODO: Lägg till

	void attach(GLuint texUnit, const std::string& ID);
	void detach(GLuint texUnit);

	std::map<GLuint, std::string> textureMap;
};
