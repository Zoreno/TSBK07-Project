#include <Engine.h>

int main()
{
	engine::Engine engine;

	engine.init();

	engine.run();

	engine.cleanup();
}