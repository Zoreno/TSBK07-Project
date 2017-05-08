/**
 * @file	UIElement.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	User interface element base class
 */

#include "UIElement.h"

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
}