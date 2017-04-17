#include "UIManager.h"

#include <iostream>
#include <GL/glew.h>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ShaderProgram.h"

namespace userinterface
{

	UIManager::UIManager(int width, int height)
		:
		_screenWidth{ width },
		_screenHeight{ height },
		_surface{ this, width, height } {}

	UIManager::~UIManager()
	{
		while(_children.size() > 0)
		{
			auto it = _children.begin();
			delete it->second;
			_children.erase(it);
		}
	}

	void UIManager::draw()
	{
		for (std::deque<Entry>::reverse_iterator it = _children.rbegin(); it != _children.rend(); ++it)
		{
			it->second->draw(&_surface);
		}
	}
}
