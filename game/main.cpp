#include <Engine.h>

#include <Component.h>
#include <Entity.h>
#include <iostream>

int main()
{
	engine::Engine engine;

	engine.init();

	engine.run();

	engine.cleanup();
}