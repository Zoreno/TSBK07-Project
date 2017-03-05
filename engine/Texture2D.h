#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureFile.h"

#include <map>

#include "Color.h"
#include "EngineDLL.h"

// Texture1D
// Texture2D
// Texture3D
// TextureCubeMap
// Texture1DArray
// Texture2DArray
// Texture3DArray
// TextureCubeMapArray

enum class TEXTURE_2D_TEXTURE_MODE
{
	STENCIL_INDEX,
	DEPTH_COMPONENT,
	INVALID_VALUE
};

enum class TEXTURE_2D_COMPARE_FUNC
{
	LESS_EQUAL,
	GREATER_EQUAL,
	LESS,
	GREATER,
	EQUAL,
	NOT_EQUAL,
	ALWAYS,
	NEVER,
	INVALID_VALUE
};

enum class TEXTURE_2D_COMPARE_MODE
{
	COMPARE_REF_TO_TEXTURE,
	NONE,
	INVALID_VALUE
};

enum class TEXTURE_2D_SWIZZLE_COMPONENT
{
	RED,
	GREEN,
	BLUE,
	ALPHA,
	ZERO,
	ONE,
	INVALID_VALUE
};

enum class TEXTURE_2D_WRAP
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER,
	INVALID_VALUE
};

enum class TEXTURE_2D_FILTERING
{
	NEAREST,
	LINEAR,
	NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR,
	INVALID_VALUE
};

class ENGINE_API Texture2D
{
public:

	Texture2D();

	explicit Texture2D(
		const char* filePath,
		TEXTURE_2D_WRAP sWrap = TEXTURE_2D_WRAP::REPEAT,
		TEXTURE_2D_WRAP tWrap = TEXTURE_2D_WRAP::REPEAT,
		TEXTURE_2D_FILTERING magFilter = TEXTURE_2D_FILTERING::LINEAR,
		TEXTURE_2D_FILTERING minFilter = TEXTURE_2D_FILTERING::LINEAR_MIPMAP_LINEAR
	);

	~Texture2D();

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	Texture2D(Texture2D&& other) noexcept;

	/**
	 * @brief Returns the width of the source image.
	 * @return Width.
	 */
	GLuint getWidth() const;

	/**
	* @brief Returns the height of the source image.
	* @return Height.
	*/
	GLuint getHeight() const;

	/**
	 * @brief Returns the openGL texture handle.
	 * @return Texture Handle.
	 */
	GLuint getHandle() const;
	
	/**
	 * @brief Depth-stencil format texture read mode.
	 * 
	 * Specifies the mode used to read from depth-stencil format textures.
	 * 
	 * @param mode Desired mode.
	 */
	void setDepthStencilTextureMode(TEXTURE_2D_TEXTURE_MODE mode);

	/**
	 * @brief Texture border color.
	 * 
	 * Defines the color to be sampled outside the texture if wrap mode is
	 * set to CLAMP_TO_BORDER.
	 * 
	 * @param color Border Color. 
	 */
	void setTextureBorderColor(Color color);

	/**
	 * @brief Texture Base Level
	 * 
	 * Specifies the index of the lowest defined mipmap level.
	 * 
	 * @param baseLevel Index.
	 */
	void setTextureBaseLevel(GLint baseLevel);

	/**
	* @brief Texture Max Level
	*
	* Specifies the index of the highest defined mipmap level.
	*
	* @param maxLevel Index.
	*/
	void setTextureMaxLevel(GLint maxLevel);

	void setTextureCompareFunc(TEXTURE_2D_COMPARE_FUNC func);

	void setTextureWrapS(TEXTURE_2D_WRAP sWrap);
	void setTextureWrapT(TEXTURE_2D_WRAP tWrap);
	void setTextureWrapR(TEXTURE_2D_WRAP rWrap);

	void setTextureMagFilter(TEXTURE_2D_FILTERING magFilter);
	void setTextureMinFilter(TEXTURE_2D_FILTERING minFilter);

	TEXTURE_2D_TEXTURE_MODE getDepthStencilTextureMode() const;

	Color getTextureBorderColor() const;

	GLint getTextureBaseLevel() const;

	GLint getTextureMaxLevel() const;

	TEXTURE_2D_COMPARE_FUNC getTextureCompareFunc() const;

	TEXTURE_2D_WRAP getTextureWrapS() const;
	TEXTURE_2D_WRAP getTextureWrapT() const;
	TEXTURE_2D_WRAP getTextureWrapR() const;

	TEXTURE_2D_FILTERING getTextureMagFilter() const;
	TEXTURE_2D_FILTERING getTextureMinFilter() const;

	// Bind texture to currently active unit
	void bind() const;

	// Bind texture to given unit
	void bind(GLuint texUnit) const;
private:
	GLuint width;
	GLuint height;

	GLuint textureID;

	friend void swap(Texture2D& first, Texture2D& second) noexcept;
};

typedef std::map<std::string, Texture2D*> Texture2DMap;