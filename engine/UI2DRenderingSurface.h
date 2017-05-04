#pragma once

#include <string>

#include "ShaderProgram.h"
#include "Color.h"
#include "UITextRenderer.h"

namespace userinterface
{
	class UIManager;

	class UI2DRenderingSurface
	{
	public:
		UI2DRenderingSurface(UIManager* manager, int width, int height);
		~UI2DRenderingSurface();

		void renderQuad(int posX, int posY, int width, int height, Color color);

		void renderText(const std::string& str, int x, int y, float scale, Color color);
	private:
		UIManager* _manager;
		int _width;
		int _height;

		ShaderProgram _shader;
		glm::mat4 _projection;

		// Rendering primitives
		// Quad
		GLuint _quadVAO;
		GLuint _quadVBO;

		// Text
		UITextRenderer _textRenderer;
		Font _font;

	};
}