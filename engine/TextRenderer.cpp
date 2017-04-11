#include "TextRenderer.h"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

// https://learnopengl.com/#!In-Practice/Text-Rendering

TextRenderer::TextRenderer(const char* fontPath)
	: textShader("../res/shaders/textVert.shader", "../res/shaders/textFrag.shader")
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cerr << "Could not initialize FT" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, fontPath, 0, &face))
		std::cerr << "Could not load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0;c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cerr << "Failed to load glyph: " << c << std::endl;
			continue;
		}

		std::shared_ptr<Texture2D> tex{ new Texture2D{
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			TEXTURE_2D_FORMAT::RED,
			TEXTURE_2D_DATATYPE::UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		} };

		tex->setTextureWrapS(TEXTURE_2D_WRAP::CLAMP_TO_EDGE);
		tex->setTextureWrapT(TEXTURE_2D_WRAP::CLAMP_TO_EDGE);
		tex->setTextureMinFilter(TEXTURE_2D_FILTERING::LINEAR);
		tex->setTextureMagFilter(TEXTURE_2D_FILTERING::LINEAR);

		Character character = {
			tex,
			glm::ivec2{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
			glm::ivec2{ face->glyph->bitmap_left, face->glyph->bitmap_top },
			face->glyph->advance.x
		};

		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	projection = glm::ortho(0.f, 800.f, 0.f, 600.f);

	try
	{
		textShader.compile();

		textShader.bindAttribLocation(0, "vertex");

		textShader.link();
	} catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		throw;
	}
}

void TextRenderer::render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, Color color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	textShader.use();

	textShader.uploadUniform("textColor", glm::vec3(color.r, color.g, color.b));
	textShader.uploadUniform("projection", projection);
	textShader.uploadUniform("text", 0);
	
	glBindVertexArray(VAO);

	std::string::const_iterator ci;

	for(ci = text.begin(); ci != text.end(); ++ci)
	{
		Character ch = Characters[*ci];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		ch.tex->bind(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
}
