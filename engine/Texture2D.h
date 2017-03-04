#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureFile.h"

#include <map>

enum TEXTURE2DWRAP
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

enum TEXTURE2DFILTERING
{
	NEAREST,
	LINEAR,
	NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR
};

class Texture2D
{
public:

	Texture2D();

	explicit Texture2D(
		const char* filePath,
		TEXTURE2DWRAP sWrap = REPEAT,
		TEXTURE2DWRAP tWrap = REPEAT,
		TEXTURE2DFILTERING magFilter = LINEAR,
		TEXTURE2DFILTERING minFilter = LINEAR_MIPMAP_LINEAR
	);

	~Texture2D();

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	Texture2D(Texture2D&& other) noexcept;

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	GLuint getHandle() const;

	// Bind texture to currently active unit
	void bind() const;

	// Bind texture to given unit
	void bind(GLuint texUnit) const;
private:
	uint32_t width;
	uint32_t height;

	GLuint textureID;

	friend void swap(Texture2D& first, Texture2D& second) noexcept;
};

typedef std::map<std::string, Texture2D*> TextureMap;

