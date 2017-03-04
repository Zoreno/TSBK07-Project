#include "Texture2D.h"

#include "TGA.h"
#include "BMP.h"

#include <string>
#include <iostream>

Texture2D::Texture2D()
	:width{ 0 }, height{ 0 }, textureID{ 0 } {}

Texture2D::Texture2D(
	const char* filePath, 
	TEXTURE2DWRAP sWrap, 
	TEXTURE2DWRAP tWrap, 
	TEXTURE2DFILTERING magFilter, 
	TEXTURE2DFILTERING minFilter)
{
	// TODO: Better way of detecting file format.

	TextureFile* file;

	size_t filePathLength = strlen(filePath);

	if (strcmp(filePath + filePathLength - 4, ".tga") == 0) // File is TGA
	{
		file = new TGA{ filePath };
	}
	else if (strcmp(filePath + filePathLength - 4, ".bmp") == 0) // File is Bitmap
	{
		file = new BMP{ filePath };
	}
	else
	{
		throw std::invalid_argument(std::string("Invalid file format. (") + filePath + ")");
	}

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,							// Target
		0,										// Level
		file->hasAlpha() ? GL_RGBA : GL_RGB,	// Internal Format
		file->getWidth(),						// Width
		file->getHeight(),						// Height
		0,										// Border
		file->hasAlpha() ? GL_RGBA : GL_RGB,	// Format
		GL_UNSIGNED_BYTE,						// Type
		file->getPixels().data());				// Pixels

	// Setup S coordinate wrap
	switch (sWrap)
	{
	case REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	case MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		break;
	case CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		break;
	case CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		break;
	}

	// Setup T coordinate wrap
	switch (tWrap)
	{
	case REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}


	switch (magFilter)
	{
	case NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		std::cerr << "Invalid GL_TEXTURE_MAG_FILTER parameter to texture " << filePath << std::endl;
		break;
	}

	switch (minFilter)
	{
	case NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case NEAREST_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case LINEAR_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case NEAREST_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case LINEAR_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	width = file->getWidth();
	height = file->getHeight();

	// We are now done with file.
	delete file;
}

Texture2D::~Texture2D()
{
	if (textureID != 0)
		glDeleteTextures(1, &textureID);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
	swap(*this, other);
}

uint32_t Texture2D::getWidth() const
{
	return width;
}

uint32_t Texture2D::getHeight() const
{
	return height;
}

GLuint Texture2D::getHandle() const
{
	return textureID;
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture2D::bind(GLuint texUnit) const
{
	GLint numTextureUnits;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureUnits);

	if(texUnit >= numTextureUnits)
	{
		throw std::invalid_argument("Requested texture unit larger than supported");
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);	
}

void swap(Texture2D& first, Texture2D& second) noexcept
{
	std::swap(first.textureID, second.textureID);
	std::swap(first.width, second.width);
	std::swap(first.height, second.height);
}
