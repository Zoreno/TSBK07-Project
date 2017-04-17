#pragma once
#include "UIElement.h"

namespace userinterface
{
	class UIProgressBar : public UIElement
	{
	public:
		UIProgressBar(
			UIManager* manager, 
			const std::string& identifier, 
			int width, 
			int height, 
			int posX, 
			int posY, 
			float initialValue, 
			float minValue, 
			float maxValue);
		~UIProgressBar();

		void draw(UI2DRenderingSurface* surface) override;

		void setFillColor(Color color);
		void setBarColor(Color color);
		void setBorderColor(Color color);
		void setTextColor(Color color);
		void setText(const std::string& text);
		void setBorder(int border);
		void setMaxValue(float value);
		void setMinValue(float value);
		void setValue(float value);

		Color getFillColor() const;
		Color getBarColor() const;
		Color getBorderColor() const;
		Color getTextColor() const;
		std::string getText() const;
		int getBorder() const;
		float getMaxValue() const;
		float getMinValue() const;
		float getValue() const;

		void incrementValue(float value);
		void decrementValue(float value);
	private:

		Color _fillColor{};
		Color _barColor{};
		Color _borderColor{};
		Color _textColor{};
		std::string _text{};
		int _border{ 0 };

		float _minValue{};
		float _maxValue{};
		float _value{};
	};
}
