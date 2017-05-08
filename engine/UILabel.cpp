/**
 * @file	UILabel.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	UI Label class
 */

#include "UILabel.h"

namespace userinterface
{
	UILabel::UILabel(
		UIManager* manager,
		const std::string& identifier,
		int width,
		int height,
		int posX,
		int posY)
		:
		UIElement{
			manager,
			identifier,
			width,
			height,
			posX,
			posY }
	{

	}

	UILabel::~UILabel()
	{

	}

	void UILabel::draw(UI2DRenderingSurface* surface)
	{
		if (!visible)
			return;

		if (_border != 0)
		{
			surface->renderQuad(
				_posX,
				_posY,
				_width,
				_height,
				_borderColor);
		}

		surface->renderQuad(
			_posX + _border,
			_posY + _border,
			_width - 2 * _border,
			_height - 2 * _border,
			_fillColor);

		surface->renderText(
			_text,
			_posX + 2 * _border + 4,
			_posY + (_height >> 1) - 8,
			1.f,
			_textColor);
	}

	void UILabel::setFillColor(Color color)
	{
		_fillColor = color;
	}

	void UILabel::setBorderColor(Color color)
	{
		_borderColor = color;
	}

	void UILabel::setTextColor(Color color)
	{
		_textColor = color;
	}

	void UILabel::setText(const std::string& text)
	{
		_text = text;
	}

	void UILabel::setBorder(int border)
	{
		_border = border;
	}

	Color UILabel::getFillColor() const
	{
		return _fillColor;
	}

	Color UILabel::getBorderColor() const
	{
		return _borderColor;
	}

	Color UILabel::getTextColor() const
	{
		return _textColor;
	}

	std::string UILabel::getText() const
	{
		return _text;
	}

	int UILabel::getBorder() const
	{
		return _border;
	}
}
