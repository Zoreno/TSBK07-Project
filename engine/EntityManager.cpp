/**
* @file	EntityManager.cpp
* @Author	Kevin Kjellén
* @date	2017-03-05
* @brief	Container for all the components for all entities
*/

#include "EntityManager.h"

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

bool EntityManager::match(EntityHandle entHandle, ComponentSet set)
{
	EntityPtr ePtr = getEntity(entHandle);

	std::cout << set << std::endl;
	std::cout << ePtr->_components << std::endl;

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
