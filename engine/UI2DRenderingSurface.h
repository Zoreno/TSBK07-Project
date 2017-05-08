/**
 * @file	UI2DRenderingSurface.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	2D Rendering surface
 */

#pragma once

#include <string>

#include "ShaderProgram.h"
#include "Color.h"
#include "UITextRenderer.h"

namespace userinterface
{
	class UIManager;

	/**
	 * @brief 2D Rendering surface.
	 */
	class UI2DRenderingSurface
	{
	public:

		/**
		 * @brief Constructor
		 * @param manager Pointer to UI manager
		 * @param width Width of surface
		 * @param height Height of surface
		 */
		UI2DRenderingSurface(UIManager* manager, int width, int height);
		
		/**
		 * @brief Destructor
		 */
		virtual ~UI2DRenderingSurface();

		/**
		 * @brief Render a quad primitive
		 * @param posX x-position
		 * @param posY y-position
		 * @param width width
		 * @param height height
		 * @param color color
		 */
		void renderQuad(int posX, int posY, int width, int height, Color color);

		/**
		 * @brief Render a text string
		 * @param str String to render
		 * @param x x-position
		 * @param y y-position
		 * @param scale Scale
		 * @param color Color
		 */
		void renderText(const std::string& str, int x, int y, float scale, Color color);
	private:
		
		/**
		 * @brief Pointer to UI manager
		 */
		UIManager* _manager;
		
		/**
		 * @brief Width of surface
		 */
		int _width;
		
		/**
		 * @brief Height of surface
		 */
		int _height;

		/**
		 * @brief UI shader
		 */
		ShaderProgram _shader;
		
		/**
		 * @brief UI projection matrix
		 */
		glm::mat4 _projection;

		/**
		 * @brief Quad primitive VAO
		 */
		GLuint _quadVAO;
		
		/**
		 * @brief Quad primitive VBO
		 */
		GLuint _quadVBO;

		/**
		 * @brief Text rendering helper object
		 */
		UITextRenderer _textRenderer;
		
		/**
		 * @brief Font to be used for text rendering.
		 */
		Font _font;
	};
}
