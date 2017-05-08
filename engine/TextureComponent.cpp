#include "TextureComponent.h"

TextureComponent::TextureComponent()
	: textureMap{} {}

void TextureComponent::attach(GLuint texUnit, const std::string& ID)
{
	textureMap.emplace(texUnit, ID);
}

void TextureComponent::detach(GLuint texUnit)
{
	auto it = textureMap.find(texUnit);

	if(it != textureMap.end())
	{
		textureMap.erase(it);
	}
}
