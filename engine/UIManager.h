#pragma once

#include "UIDetail.h"

#include <string>
#include "Color.h"
#include <GL/glew.h>
#include "ShaderProgram.h"
#include "UITextRenderer.h"
#include "UI2DRenderingSurface.h"
#include "UIElement.h"
#include <deque>

namespace userinterface
{
	class ENGINE_API UIManager
	{
	public:
		UIManager(int width, int height);
		~UIManager();

		template <typename T>
		typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
			getElement(const std::string& identifier);

		template <typename T, typename ... Args>
		typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
			addElement(const std::string& identifier, Args ... args);

		void draw();
	private:
		typedef std::pair<std::string, UIElement*> Entry;

		std::deque<Entry> _children;

		int _screenWidth;
		int _screenHeight;

		UI2DRenderingSurface _surface;
	};

	template <typename T>
	typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
		UIManager::getElement(const std::string& identifier)
	{
		for (auto it = _children.begin();it != _children.end(); ++it)
		{
			if(it->first == identifier)
			{
				return dynamic_cast<T*>(it->second);
			}
		}
		return nullptr;
	}

	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
		UIManager::addElement(const std::string& identifier, Args ... args)
	{
		_children.emplace_front(Entry(identifier, new T(this, identifier, std::forward<Args>(args)...)));

		return dynamic_cast<T*>(_children.front().second);
	}
}

