#pragma once

#include "UIDetail.h"

#include <string>
#include "Color.h"

namespace userinterface
{
	class UIManager;

	class UIElement : public detail::IntRect
	{
	public:
		UIElement() = delete;
		UIElement(UIManager* manager, const std::string& identifier, int width, int height, int posX, int posY);

		// TODO: Copy/Move ctor/=

		virtual ~UIElement();

		virtual void draw() = 0;

		std::string getIdentifier() const;

		void show();
		void hide();
		void toggleVisibility();

	protected:
		UIManager* manager;
		std::string identifier;

		bool visible{ true };
		bool active{ false };
	};

	class UITestRect : public UIElement
	{
	public:
		UITestRect(UIManager* manager, const std::string& identifier);

		void draw() override;

		Color color{ 1.f,0.f,1.f };
	};

	class UIManager
	{
	public:
		UIManager(int width, int height);

		template <typename T>
		typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
			getElement(const std::string& identifier);

		template <typename T, typename ... Args>
		typename std::enable_if<std::is_base_of<UIElement, T>::value>::type
			addElement(const std::string& identifier, Args ... args);

		void draw();
	private:
		std::map<std::string, UIElement*> children;

		int screenWidth;
		int screenHeight;
	};

	template <typename T>
	typename std::enable_if<std::is_base_of<UIElement, T>::value, T*>::type
		UIManager::getElement(const std::string& identifier)
	{
		auto it = children.find(identifier);

		if (it != children.end())
		{
			return dynamic_cast<T*>(it->second);
		}

		return nullptr;
	}

	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<UIElement, T>::value>::type
		UIManager::addElement(const std::string& identifier, Args ... args)
	{
		children.emplace(identifier, new T{ this, identifier, std::forward<Args>(args)... });
	}
}

