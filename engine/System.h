#pragma once

#include <vector>

class System
{
public:
	System() = default;
	virtual ~System() {}

	virtual void update(float dt) = 0;
protected:

private:
};

class SystemManager
{
public:

	SystemManager() {}
	~SystemManager();

	void update(float dt);

	
	template <typename T, typename ... Args>
	void registerSystem(Args&& ... args);

private:
	
	void regSystem(System* system);
	
	std::vector<System*> systems{};

};

template <typename T, typename ... Args>
void SystemManager::registerSystem(Args&&... args)
{
	regSystem(new T(std::forward<Args>(args)...));
}
