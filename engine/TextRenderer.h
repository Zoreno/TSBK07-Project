#pragma once
#include <string>

#include <glm/glm.hpp>
#include "Color.h"
#include "Texture2D.h"
#include <memory>
#include "ShaderProgram.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	std::shared_ptr<Texture2D> texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

typedef std::map<GLchar, Character> CharMap;

struct Font
{
	CharMap characters;
};

class TextRenderer
{
public:
	explicit TextRenderer( unsigned int witdth, unsigned int height);
	~TextRenderer();

	void loadFont(const char* fontPath, unsigned int size, Font* font);

	void render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, Color color, Font* font);

	void setScreenDimensions(unsigned int width, unsigned int height);
private:

	GLuint VAO;
	GLuint VBO;
	ShaderProgram textShader;

	FT_Library ft;

	unsigned int screenWidth;
	unsigned int screenHeight;

	glm::mat4 projection;
};