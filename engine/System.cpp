#include "System.h"

SystemManager::~SystemManager()
{
	while(systems.size() > 0)
	{
		delete systems.back();
		systems.pop_back();
	}
}

void SystemManager::update(float dt)
{
	for(auto& it : systems)
	{
		it->update(dt);
	}
}

void SystemManager::regSystem(System* system)
{
	systems.push_back(system);
}
