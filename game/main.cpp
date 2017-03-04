#include "Engine.h"

#include "Window.h"

int main()
{
	engine::Engine engine;

	engine.init();

	engine.run();

	engine.cleanup();
}