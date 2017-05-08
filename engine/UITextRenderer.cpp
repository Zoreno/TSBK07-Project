/**
 * @file	UITextRenderer.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	UI Text Rendering Helper Class
 */

#include "UITextRenderer.h"

#include <iostream>

// https://learnopengl.com/#!In-Practice/Text-Rendering

namespace userinterface
{
	UITextRenderer::UITextRenderer(unsigned int width, unsigned int height)
		:
		_textShader("../res/shaders/textVert.shader", "../res/shaders/textFrag.shader"),
		_screenWidth{ width },
		_screenHeight{ height }
	{
		if (FT_Init_FreeType(&_ft))
			std::cerr << "Could not initialize FT" << std::endl;

		// Setup rendering quad
		glGenVertexArrays(1, &_VAO);
		glGenBuffers(1, &_VBO);
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Setup screen projection.
		_projection = glm::ortho(0.f, static_cast<GLfloat>(_screenWidth), 0.f, static_cast<GLfloat>(_screenHeight));

		// Setup shader
		try
		{
			_textShader.compile();

			_textShader.bindAttribLocation(0, "vertex");

			_textShader.link();
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
			throw;
		}
	}

	UITextRenderer::~UITextRenderer()
	{
		FT_Done_FreeType(_ft);
	}

	void UITextRenderer::loadFont(const char* fontPath, unsigned int size, Font* font) const
	{
		FT_Face face;
		if (FT_New_Face(_ft, fontPath, 0, &face))
			std::cerr << "Could not load font" << std::endl;

		FT_Set_Pixel_Sizes(face, 0, size);

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

			font->characters.emplace(c, character);
		}

		FT_Done_Face(face);
	}

	void UITextRenderer::render(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, Color color, Font* font)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		_textShader.use();

		_textShader.uploadUniform("textColor", glm::vec3(color.r, color.g, color.b));
		_textShader.uploadUniform("projection", _projection);
		_textShader.uploadUniform("text", 0);

		glBindVertexArray(_VAO);

		std::string::const_iterator ci;

		for (ci = text.begin(); ci != text.end(); ++ci)
		{
			Character ch = font->characters[*ci];

			GLfloat xpos = x + ch.bearing.x * scale;
			GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

			GLfloat w = ch.size.x * scale;
			GLfloat h = ch.size.y * scale;

			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};

			ch.texture->bind(0);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * scale;
		}

		glBindVertexArray(0);
	}

	void UITextRenderer::setScreenDimensions(unsigned int width, unsigned int height)
	{
		_screenWidth = width;
		_screenHeight = height;

		_projection = glm::ortho(
			0.f, 
			static_cast<GLfloat>(_screenWidth), 
			0.f,
			static_cast<GLfloat>(_screenHeight));
	}

}