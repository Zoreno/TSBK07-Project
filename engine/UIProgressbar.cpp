/**
 * @file	UIProgressbar.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	UI Progress bar
 */

#include "UIProgressBar.h"

namespace userinterface
{
	UIProgressBar::UIProgressBar(
		UIManager* manager,
		const std::string& identifier,
		int width,
		int height,
		int posX,
		int posY,
		float initialValue,
		float minValue,
		float maxValue)
		:
		UIElement(
			manager, 
			identifier,
			width,
			height, 
			posX, 
			posY),
		_minValue(minValue),
		_maxValue(maxValue),
		_value(initialValue)
	{
	}

	UIProgressBar::~UIProgressBar()
	{
	}

	void UIProgressBar::draw(UI2DRenderingSurface* surface)
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

		float fillLevel = (_value - _minValue) / (_maxValue - _minValue);

		surface->renderQuad(
			_posX + _border, 
			_posY + _border, 
			(_width - 2 * _border) * fillLevel,
			_height - 2 * _border, 
			_barColor);

		surface->renderText(
			_text,
			_posX + 2 * _border + 4,
			_posY + (_height >> 1) - 8, 
			1.f,
			_textColor);
	}

	void UIProgressBar::setFillColor(Color color)
	{
		_fillColor = color;
	}

	void UIProgressBar::setBarColor(Color color)
	{
		_barColor = color;
	}

	void UIProgressBar::setBorderColor(Color color)
	{
		_borderColor = color;
	}

	void UIProgressBar::setTextColor(Color color)
	{
		_textColor = color;
	}

	void UIProgressBar::setText(const std::string& text)
	{
		_text = text;
	}

	void UIProgressBar::setBorder(int border)
	{
		_border = border;
	}

	void UIProgressBar::setMaxValue(float value)
	{
		_maxValue = value;
	}

	void UIProgressBar::setMinValue(float value)
	{
		_minValue = value;
	}

	void UIProgressBar::setValue(float value)
	{
		_value = glm::clamp(value, _minValue, _maxValue);
	}

	Color UIProgressBar::getFillColor() const
	{
		return _fillColor;
	}

	Color UIProgressBar::getBarColor() const
	{
		return _barColor;
	}

	Color UIProgressBar::getBorderColor() const
	{
		return _borderColor;
	}

	Color UIProgressBar::getTextColor() const
	{
		return _textColor;
	}

	std::string UIProgressBar::getText() const
	{
		return _text;
	}

	int UIProgressBar::getBorder() const
	{
		return _border;
	}

	float UIProgressBar::getMaxValue() const
	{
		return _maxValue;
	}

	float UIProgressBar::getMinValue() const
	{
		return _minValue;
	}

	float UIProgressBar::getValue() const
	{
		return _value;
	}

	void UIProgressBar::incrementValue(float value)
	{
		_value = glm::clamp(_value + value, _minValue, _maxValue);
	}

	void UIProgressBar::decrementValue(float value)
	{
		_value = glm::clamp(_value - value, _minValue, _maxValue);
	}
}
