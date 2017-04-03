#pragma once

class EntityManager;
class EventManager;

typedef uint32_t EntityHandle;

class System
{
public:
	virtual ~System() {}

	virtual void registerManagers(EntityManager* em, EventManager* ev) { this->em = em; this->ev = ev; }

	virtual void startUp() {}
	virtual void shutDown() {}

	virtual void update(float dt) = 0;
protected:

	EntityManager* em{ nullptr };
	EventManager* ev{ nullptr };
};