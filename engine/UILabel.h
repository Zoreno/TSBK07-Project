/**
 * @file	UILabel.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	Class Representing an UI Label
 */

#pragma once

#include "UIElement.h"
#include <string>

namespace userinterface
{
	class UIManager;

	/**
	 * @brief Class Representing an UI label
	 */
	class UILabel : public UIElement
	{
	public:

		/**
		 * @brief Constructor
		 * @param manager Pointer to UI manager
		 * @param identifier Name of element
		 * @param width Width
		 * @param height Height
		 * @param posX X-position
		 * @param posY Y-position
		 */
		UILabel(
			UIManager* manager, 
			const std::string& identifier, 
			int width, 
			int height, 
			int posX, 
			int posY);

		/**
		 * @brief Destructor
		 */
		virtual ~UILabel();

		/**
		 * @brief Draws the element.
		 * @param surface Surface to render it to.
		 */
		virtual void draw(UI2DRenderingSurface* surface) override;

		/**
		 * @brief Sets the fill color
		 * @param color New color
		 */
		void setFillColor(Color color);
		
		/**
		 * @brief Sets the border color.
		 * @param color New color.
		 */
		void setBorderColor(Color color);
		
		/**
		 * @brief Sets the text color.
		 * @param color New Color
		 */
		void setTextColor(Color color);
		
		/**
		 * @brief Sets the text string.
		 * @param text New text.
		 */
		void setText(const std::string& text);
		
		/**
		 * @brief Sets border width.
		 * @param border Border width
		 */
		void setBorder(int border);

		/**
		 * @brief Gets the fill color.
		 * @return Fill color.
		 */
		Color getFillColor() const;
		
		/**
		 * @brief Gets the border color.
		 * @return Border color.
		 */
		Color getBorderColor() const;
		
		/**
		 * @brief Gets the text color.
		 * @return Text color.
		 */
		Color getTextColor() const;
		
		/**
		 * @brief Gets the text.
		 * @return Text.
		 */
		std::string getText() const;
		
		/**
		 * @brief Gets the border width.
		 * @return Border width.
		 */
		int getBorder() const;

	private:
		
		/**
		 * @brief Fill color.
		 */
		Color _fillColor{};
		
		/**
		 * @brief Border Color.
		 */
		Color _borderColor{};
		
		/**
		 * @brief Text Color.
		 */
		Color _textColor{};
		
		/**
		 * @brief Text.
		 */
		std::string _text{};
		
		/**
		 * @brief Border.
		 */
		int _border{ 0 };
	};
}