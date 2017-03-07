#pragma once

#include "EngineDLL.h"

#include "Window.h"

#include "Color.h"

#include "TransformPipeline3D.h"

#include <iostream>

namespace engine
{
	class ENGINE_API Engine
	{
	public:
		void init();

		void run();

		void cleanup();

	private:

		void dumpInfo(std::ostream& stream);

		Window* window{ nullptr };
	};
}