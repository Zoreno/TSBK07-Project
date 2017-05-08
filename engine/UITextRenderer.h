/**
 * @file	UITextRenderer.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	Text Renderer Class
 */

#pragma once

#include <string>

#include <glm/glm.hpp>
#include "Color.h"
#include "Texture2D.h"
#include <memory>
#include "ShaderProgram.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace userinterface
{
	/**
	 * @brief Struct representing a text character
	 */
	struct Character
	{
		/**
		 * @brief Character texture 
		 */
		std::shared_ptr<Texture2D> texture;
		
		/**
		 * @brief Size of the character
		 */
		glm::ivec2 size;
		
		/**
		 * @brief Character bearing
		 */
		glm::ivec2 bearing;
		
		/**
		 * @brief Character advance.
		 */
		GLuint advance;
	};

	/**
	 * @brief Map of Characters to their respective char codes
	 */
	typedef std::map<GLchar, Character> CharMap;

	/**
	 * @brief Struct representing a font.
	 */
	struct Font
	{
		/**
		 * @brief Map of characters.
		 */
		CharMap characters;
	};

	/**
	 * @brief Text rendering helper class.
	 */
	class UITextRenderer
	{
	public:

		/**
		 * @brief Constructor
		 * @param witdth Width of screen
		 * @param height Height of screen
		 */
		explicit UITextRenderer(unsigned int witdth, unsigned int height);

		/**
		 * @brief Destructor
		 */
		~UITextRenderer();

		/**
		 * @brief Loads a font into a font struct.
		 * @param fontPath Path to .ttf file.
		 * @param size Size of characters
		 * @param font Pointer to struct where data should be stored.
		 */
		void loadFont(const char* fontPath, unsigned int size, Font* font) const;

		/**
		 * @brief Renders a string.
		 * @param text String to render
		 * @param x X-position
		 * @param y Y-position
		 * @param scale Render scale
		 * @param color Color
		 * @param font Font to use.
		 */
		void render(
			const std::string& text, 
			GLfloat x, 
			GLfloat y,
			GLfloat scale, 
			Color color, 
			Font* font);

		/**
		 * @brief Set new screen dimensions
		 * @param width New width
		 * @param height New height
		 */
		void setScreenDimensions(unsigned int width, unsigned int height);
	private:

		/**
		 * @brief Render Quad VAO
		 */
		GLuint _VAO;

		/**
		 * @brief Render Quad VBO
		 */
		GLuint _VBO;

		/**
		 * @brief Text rendering shader.
		 */
		ShaderProgram _textShader;

		/**
		 * @brief Font loading library
		 */
		FT_Library _ft;

		/**
		 * @brief Screen Width
		 */
		unsigned int _screenWidth;

		/**
		 * @brief Screen Height
		 */
		unsigned int _screenHeight;

		/**
		 * @brief Projection matrix.
		 */
		glm::mat4 _projection;
	};
}
