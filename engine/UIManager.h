/**
 * @file	UIManager.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	UI Manager
 */

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
	/**
	 * @brief User Interface manager.
	 */
	class UIManager
	{
	public:

		/**
		 * @brief Constructor
		 * @param width Width of surface
		 * @param height Height of surface
		 */
		UIManager(int width, int height);

		/**
		 * @brief Destructor
		 */
		~UIManager();

		/**
		 * @brief Gets the element with by identifier.
		 * @tparam T Element type
		 * @param identifier Name of element
		 * @return Pointer to element if present, otherwise nullptr
		 */
		template <typename T>
		typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
			getElement(const std::string& identifier);

		/**
		 * @brief Adds an element to the UI
		 * @tparam T Type of element
		 * @tparam Args Types of arguments to forward.
		 * @param identifier Name of element
		 * @param args Arguments to forward.
		 * @return Pointer to element
		 */
		template <typename T, typename ... Args>
		typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
			addElement(const std::string& identifier, Args ... args);

		/**
		 * @brief Draw the elements to the surface.
		 */
		void draw();
	private:

		/**
		 * @brief Type definition of entries in queue.
		 */
		typedef std::pair<std::string, UIElement*> Entry;

		/**
		 * @brief Contained elements.
		 */
		std::deque<Entry> _children;

		/**
		 * @brief Width of screen
		 */
		int _screenWidth;

		/**
		 * @brief Height of screen
		 */
		int _screenHeight;

		/**
		 * @brief Surface to draw to.
		 */
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