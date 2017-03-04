#include "Engine.h"

namespace engine
{
	void Engine::init()
	{
		window = new Window{ 800,600,"MyWindow" };
	}

	void Engine::run()
	{
		while (!window->shouldClose())
		{
			window->display();
		}
	}

	void Engine::cleanup()
	{
		delete window;
	}
}
