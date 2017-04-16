#include "UIManager.h"

#include <iostream>
#include <GL/glew.h>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ShaderProgram.h"

namespace userinterface
{
	UIElement::UIElement(UIManager* manager, const std::string& identifier, int width, int height, int posX, int posY)
		: Rectangle<int>{ width, height, posX, posY }, manager{ manager }, identifier{ identifier } {}

	UIElement::~UIElement() {}

	std::string UIElement::getIdentifier() const
	{
		return identifier;
	}

	void UIElement::show()
	{
		visible = true;
	}

	void UIElement::hide()
	{
		visible = false;
	}

	void UIElement::toggleVisibility()
	{
		visible = !visible;
	}

	UITestRect::UITestRect(UIManager* manager, const std::string& identifier)
		: UIElement{ manager, identifier, 64, 64, 0, 0 } {}

	void UITestRect::draw()
	{
		if (!visible)
			return;

		GLuint VAO;
		GLuint VBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glm::mat4 projection = glm::ortho(0.f, 800.f, 0.f, 600.f);


		ShaderProgram shader{ "../res/shaders/uiVert.shader","../res/shaders/uiFrag.shader" };
		try
		{
			shader.compile();

			shader.bindAttribLocation(0, "vertex");

			shader.link();
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
			throw;
		}

		GLfloat vertices[6][4] = {
			{ _posX, _posY + _height, 0.0, 0.0 },
			{ _posX, _posY, 0.0, 1.0 },
			{ _posX + _width, _posY, 1.0, 1.0 },

			{ _posX, _posY + _height, 0.0, 0.0 },
			{ _posX + _width, _posY,       1.0, 1.0 },
			{ _posX + _width, _posY + _height,   1.0, 0.0 }
		};


		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		shader.use();

		shader.uploadUniform("color", glm::vec3(color.r, color.g, color.b));
		shader.uploadUniform("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	UIManager::UIManager(int width, int height)
		:screenWidth{ width }, screenHeight{ height } {}

	void UIManager::draw()
	{
		for (auto it : children)
		{
			it.second->draw();
		}
	}
}
