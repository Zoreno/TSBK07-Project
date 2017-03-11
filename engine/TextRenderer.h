#pragma once
#include <string>

#include <glm/glm.hpp>
#include "Color.h"
#include "Texture2D.h"
#include <memory>
#include "ShaderProgram.h"

// TODO: multiple fonts

struct Character
{
	std::shared_ptr<Texture2D> tex;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class TextRenderer
{
public:
	explicit TextRenderer(const char* fontPath);

	void render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, Color color);

private:

	std::map<GLchar, Character> Characters;

	GLuint VAO;
	GLuint VBO;
	ShaderProgram textShader;

	glm::mat4 projection;
};