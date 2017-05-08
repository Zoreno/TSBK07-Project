#pragma once

#include "UIDetail.h"
#include "UI2DRenderingSurface.h"

#include <string>


namespace userinterface
{
	class UIManager;

	class ENGINE_API UIElement : public detail::IntRect
	{
	public:
		UIElement() = delete;
		UIElement(UIManager* manager, const std::string& identifier, int width, int height, int posX, int posY);

		// TODO: Copy/Move ctor/=

		virtual ~UIElement();

		virtual void draw(UI2DRenderingSurface* surface) = 0;

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
}