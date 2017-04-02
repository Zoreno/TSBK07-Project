#pragma once

class EntityManager;

typedef uint32_t EntityHandle;

class System
{
public:
	virtual ~System() {}

	virtual void registerManager(EntityManager* em) { this->em = em; }

	virtual void update(float dt) = 0;
protected:

	EntityManager* em{ nullptr };
};