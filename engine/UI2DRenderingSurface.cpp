#include "UI2DRenderingSurface.h"

#include <iostream>

namespace userinterface
{
	UI2DRenderingSurface::UI2DRenderingSurface(UIManager* manager, int width, int height)
		:
		_manager{ manager },
		_width{ width },
		_height{ height },
		_shader{ "../res/shaders/uiVert.shader","../res/shaders/uiFrag.shader" },
		_projection{ glm::ortho(0.f, static_cast<GLfloat>(width), 0.f, static_cast<GLfloat>(height)) },
		_textRenderer{ width, height }
	{
		// Setup Quad
		glGenVertexArrays(1, &_quadVAO);
		glGenBuffers(1, &_quadVBO);
		glBindVertexArray(_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Setup Shader
		try
		{
			_shader.compile();

			_shader.bindAttribLocation(0, "vertex");

			_shader.link();
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
			throw;
		}

		// Setup text renderer

		_textRenderer.loadFont("../res/fonts/arial.ttf", 16, &_font);
	}

	UI2DRenderingSurface::~UI2DRenderingSurface()
	{
		glDeleteBuffers(1, &_quadVBO);
		glDeleteVertexArrays(1, &_quadVAO);
	}

	void UI2DRenderingSurface::renderQuad(int posX, int posY, int width, int height, Color color)
	{
		GLfloat vertices[6][4] = {
			{ posX, posY + height, 0.0, 0.0 },
			{ posX, posY, 0.0, 1.0 },
			{ posX + width, posY, 1.0, 1.0 },

			{ posX, posY + height, 0.0, 0.0 },
			{ posX + width, posY, 1.0, 1.0 },
			{ posX + width, posY + height, 1.0, 0.0 }
		};

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		_shader.use();

		_shader.uploadUniform("color", glm::vec4(color.r, color.g, color.b, color.a));
		_shader.uploadUniform("projection", _projection);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void UI2DRenderingSurface::renderText(const std::string& str, int x, int y, float scale, Color color)
	{
		_textRenderer.render(str, x, y, scale, color, &_font);
	}
}