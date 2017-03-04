#pragma once

#include "EngineDLL.h"

#include "Window.h"

namespace engine
{
	class ENGINE_API Engine
	{
	public:
		void init();

		void run();

		void cleanup();

	private:

		Window* window{ nullptr };
	};
}