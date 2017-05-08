/**
* @file		EntityManager.cpp
* @Author	Kevin Kjellén
* @date		2017-03-05
* @brief	Container for all the components for all entities
*/

#include "EntityManager.h"
#include "Utils.h"
#include "EntityCreatedEvent.h"
#include "EntityDestroyedEvent.h"

EntityHandle getNextHandle()
{
	static EntityHandle next = 1;

	return next++;
}

ComponentType ComponentTypeMap::getTypeIDFromString(const std::string& name) const
{
	auto it = _component_names.find(name);

	if (it == _component_names.end())
		throw EntityManagerException{ "Component not registered." };

	return it->second;
}

EventManager::~EventManager()
{
	while(_channels.size())
	{
		delete _channels.begin()->second;
		_channels.erase(_channels.begin());
	}
}

EntityManager::~EntityManager()
{
	while(_systems.size() > 0)
	{
		_systems.back()->shutDown();
		delete _systems.back();
		_systems.pop_back();
	}

	while (_pools.size() > 0)
	{
		delete _pools.begin()->second;
		_pools.erase(_pools.begin());
	}
}

EntityHandle EntityManager::createEntity()
{
	Entity ent{};

	_ent_to_add.push_back(ent);

	eventManager->postEvent(EntityCreatedEvent(ent.getID()));

	return ent.getID();
}

EntityHandle EntityManager::copyEntity(EntityHandle from)
{
	EntityHandle to = createEntity();

	EntityPtr ePtr = getEntity(from);

	for (size_t i{ 0 }; i < MAX_COMPONENTS; ++i)
	{
		if (ePtr->_components[i])
		{
			EntityPtr e2Ptr = getEntity(to);
			e2Ptr->_components.set(i, true);
			_pools[i]->copyComponent(from, to);
		}
	}

	return to;
}

EntityHandle EntityManager::createEntityFromFile(const char* filePath)
{
	using namespace rapidxml;

	std::string source = getStringFromFile(filePath);

	xml_document<> doc;

	doc.parse<0>(const_cast<char*>(source.c_str()));

	EntityHandle entHandle = createEntity();
	EntityPtr entPtr = getEntity(entHandle);

	xml_node<>* components = doc.first_node("components");

	xml_node<>* currComp = components->first_node("component");

	while(currComp != nullptr)
	{
		xml_attribute<>* type = currComp->first_attribute("type");

		ComponentType compType = _typemap.getTypeIDFromString(type->value());

		_pools[compType]->createComponentFromNode(entHandle, currComp);
		entPtr->_components.set(compType, true);

		currComp = currComp->next_sibling("component");
	}

	return entHandle;
}

void EntityManager::destroyEntity(EntityHandle entHandle)
{
	_ent_to_remove.push_back(entHandle);

	eventManager->postEvent(EntityDestroyedEvent(entHandle));
}

void EntityManager::refresh()
{
	// Add pending entities.
	for(auto entity : _ent_to_add)
	{
		_entities.push_back(entity);
	}

	_ent_to_add.clear();

	// Remove pending entities.
	for (auto entHandle : _ent_to_remove)
	{
		EntityPtr ePtr = getEntity(entHandle);

		for (size_t i{ 0 }; i < MAX_COMPONENTS; ++i)
		{
			if (ePtr->_components[i])
			{
				_pools[i]->removeComponent(entHandle);
			}
		}

		for (auto it = _entities.begin(); it != _entities.end(); ++it)
		{
			if (it->getID() == entHandle)
			{
				_entities.erase(it);
				break;
			}
		}
	}

	_ent_to_remove.clear();
}

void EntityManager::update(float dt)
{
	for(auto it : _systems)
	{
		it->update(dt);
	}

	refresh();
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

	for(auto it = _ent_to_add.begin(); it != _ent_to_add.end(); ++it)
	{
		if(it->getID() == entHandle)
		{
			return EntityPtr(it._Ptr);
		}
	}

	return nullptr;
}
