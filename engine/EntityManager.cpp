/**
* @file	EntityManager.cpp
* @Author	Kevin Kjellén
* @date	2017-03-05
* @brief	Container for all the components for all entities
*/

#include "EntityManager.h"

EntityHandle getNextHandle()
{
	static EntityHandle next = 0;

	return next++;
}

EntityManager::~EntityManager()
{
	while(_pools.size() > 0)
	{
		delete _pools.begin()->second;
		_pools.erase(_pools.begin());
	}
}

EntityHandle EntityManager::createEntity()
{
	Entity ent{};

	_entities.push_back(ent);

	return ent.getID();
}

void EntityManager::destroyEntity(EntityHandle entHandle)
{
	EntityPtr ePtr = getEntity(entHandle);

	for(size_t i{0}; i < MAX_COMPONENTS; ++i)
	{
		if(ePtr->_components[i])
		{
			_pools[i]->removeComponent(entHandle);
		}
	}

	for(auto it = _entities.begin(); it != _entities.end(); ++it)
	{
		if(it->getID() == entHandle)
		{
			_entities.erase(it);
			break;
		}
	}
}

bool EntityManager::match(EntityHandle entHandle, ComponentSet set)
{
	EntityPtr ePtr = getEntity(entHandle);

	return (ePtr->_components & set) == set;
}

EntityPtr EntityManager::getEntity(EntityHandle entHandle)
{
	for (auto it = _entities.begin(); it != _entities.end(); ++it)
	{
		if (it->getID() == entHandle)
		{
			return EntityPtr(it._Ptr);
		}

	}

	return nullptr;
}
