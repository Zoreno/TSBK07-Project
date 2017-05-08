#pragma once

#include "UIElement.h"
#include <string>

namespace userinterface
{
	class UIManager;

	class UILabel : public UIElement
	{
	public:
		UILabel(UIManager* manager, const std::string& identifier, int width, int height, int posX, int posY);
		~UILabel();

		void draw(UI2DRenderingSurface* surface) override;

		void setFillColor(Color color);
		void setBorderColor(Color color);
		void setTextColor(Color color);
		void setText(const std::string& text);
		void setBorder(int border);

		Color getFillColor() const;
		Color getBorderColor() const;
		Color getTextColor() const;
		std::string getText() const;
		int getBorder() const;

	private:
		Color _fillColor{};
		Color _borderColor{};
		Color _textColor{};
		std::string _text{};
		int _border{ 0 };
	};
}