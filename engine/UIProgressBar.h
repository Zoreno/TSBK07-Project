/**
 * @file	UIProgressBar.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	UI Progress bar
 */

#pragma once

#include "UIElement.h"

namespace userinterface
{
	/**
	 * @brief UI Progress Bar class
	 */
	class UIProgressBar : public UIElement
	{
	public:
		/**
		 * @brief Constructor
		 * @param manager Pointer to UI Manager
		 * @param identifier Name of element
		 * @param width Width
		 * @param height Height
		 * @param posX X-position
		 * @param posY Y-position
		 * @param initialValue Initial Value
		 * @param minValue Min Value
		 * @param maxValue Max Value
		 */
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

		/**
		 * @brief Destructor
		 */
		virtual ~UIProgressBar();

		/**
		 * @brief Draws the progress bar.
		 * @param surface Surface to render it to.
		 */
		virtual void draw(UI2DRenderingSurface* surface) override;

		/**
		 * @brief Sets fill color.
		 * @param color New color.
		 */
		void setFillColor(Color color);
		
		/**
		 * @brief Sets bar color.
		 * @param color New color.
		 */
		void setBarColor(Color color);
		
		/**
		 * @brief Sets border color.
		 * @param color New color.
		 */
		void setBorderColor(Color color);
		
		/**
		 * @brief Sets text color.
		 * @param color New color.
		 */
		void setTextColor(Color color);
		
		/**
		 * @brief Sets text.
		 * @param text New text.
		 */
		void setText(const std::string& text);
		
		/**
		 * @brief Sets border width.
		 * @param border Border width.
		 */
		void setBorder(int border);
		
		/**
		 * @brief Sets the max value.
		 * @param value New max value.
		 */
		void setMaxValue(float value);
		
		/**
		 * @brief Sets the new min value.
		 * @param value New min value.
		 */
		void setMinValue(float value);
		
		/**
		 * @brief Sets the new value.
		 * @param value New value.
		 */
		void setValue(float value);

		/**
		 * @brief Gets the fill color.
		 * @return Fill color.
		 */
		Color getFillColor() const;
		
		/**
		 * @brief Gets the bar color.
		 * @return Bar color.
		 */
		Color getBarColor() const;
		
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
		 * @brief Gets text.
		 * @return Text.
		 */
		std::string getText() const;
		
		/**
		 * @brief Get border width.
		 * @return Border width-
		 */
		int getBorder() const;
		
		/**
		 * @brief Gets max value.
		 * @return Max value.
		 */
		float getMaxValue() const;
		
		/**
		 * @brief Gets min value.
		 * @return Min value.
		 */
		float getMinValue() const;
		
		/**
		 * @brief Gets value.
		 * @return Value.
		 */
		float getValue() const;

		/**
		 * @brief Increment value.
		 * @param value Value to increment it by.
		 */
		void incrementValue(float value);
		
		/**
		 * @brief Decrement value.
		 * @param value Value to decrement it by.
		 */
		void decrementValue(float value);
	private:

		/**
		 * @brief Fill Color.
		 */
		Color _fillColor{};
		
		/**
		 * @brief Bar color.
		 */
		Color _barColor{};
		
		/**
		 * @brief Border color.
		 */
		Color _borderColor{};
		
		/**
		 * @brief Text color.
		 */
		Color _textColor{};
		
		/**
		 * @brief Text.
		 */
		std::string _text{};
		
		/**
		 * @brief Border Width.
		 */
		int _border{ 0 };

		/**
		 * @brief Min Value.
		 */
		float _minValue{};
		
		/**
		 * @brief Max Value.
		 */
		float _maxValue{};
		
		/**
		 * @brief Current Value.
		 */
		float _value{};
	};
}
