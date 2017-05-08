/**
 * @file	UIElement.h
 * @Author	Joakim Bertils
 * @date	2017-05-08
 * @brief	Base Class of User Interface elements
 */

#pragma once

#include "UIDetail.h"
#include "UI2DRenderingSurface.h"

#include <string>

namespace userinterface
{
	class UIManager;

	/**
	 * @brief Base class of User Interface elements
	 */
	class UIElement : public detail::IntRect
	{
	public:

		/**
		 * @brief Default constructor
		 */
		UIElement() = delete;

		/**
		 * @brief Constructor
		 * @param manager Pointer to UI manager
		 * @param identifier Name of element
		 * @param width Width
		 * @param height Height
		 * @param posX X-position
		 * @param posY Y-position
		 */
		UIElement(
			UIManager* manager, 
			const std::string& identifier, 
			int width, 
			int height, 
			int posX, 
			int posY);

		/**
		 * @brief Destructor
		 */
		virtual ~UIElement();

		/**
		 * @brief Draw the element
		 * @param surface Surface to render to.
		 */
		virtual void draw(UI2DRenderingSurface* surface) = 0;

		/**
		 * @brief Gets the name of the element.
		 * @return Identifier
		 */
		std::string getIdentifier() const;

		/**
		 * @brief Shows the element
		 */
		void show();

		/**
		 * @brief Hides the element
		 */
		void hide();
		
		/**
		 * @brief Toggles the visibility of the element
		 */
		void toggleVisibility();

	protected:

		/**
		 * @brief Pointer to the UI manager
		 */
		UIManager* manager;

		/**
		 * @brief Identifier
		 */
		std::string identifier;

		/**
		 * @brief Visibility flag
		 */
		bool visible{ true };

		/**
		 * @brief Active flag.
		 */
		bool active{ false };
	};
}