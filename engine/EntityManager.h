/**
* @file	EntityManager.h
* @Author	Kevin Kjellén
* @date	2017-03-05
* @brief	Container for all the components for all entities
*/

#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <typeindex>
#include <memory>

#include "Entity.h"

#define USE_ASSERT 1

#if USE_ASSERT == 1
#define EM_ASSERT(expr, message) if(!(expr)) {std::cerr << "ASSERTION FAILED: " << __FILE__ << ":" << __LINE__ << "[" << message << "]" << std::endl;}
#else
#define EM_ASSERT(expr, message)
#endif

typedef uint32_t EntityHandle;
typedef uint32_t ComponentType;
typedef size_t ComponentHash;

class BasePool
{
public:
	virtual ~BasePool() {}
};

typedef std::map<ComponentType, BasePool*> PoolMap;

template <typename T>
class ComponentPool : public BasePool
{
public:
	ComponentPool() = default;
	~ComponentPool() override {}

	T* getComponent(EntityHandle entHandle);

	template <typename ... Args>
	void createComponent(EntityHandle entHandle, Args ... args);

	void removeComponent(EntityHandle entHandle);
private:

	std::map<EntityHandle, T> _components{};
};

class ComponentTypeMap
{
public:
	ComponentTypeMap() = default;
	~ComponentTypeMap() {}

	template <typename T>
	ComponentType getTypeID();

	template <typename T>
	ComponentType createTypeID();

	const ComponentType INVALID_TYPE{ 0 };

private:
	ComponentType _currID{ 1 };
	std::map<ComponentHash, ComponentType> _component_types{};
};

class EntityManager
{
public:
	EntityManager() {}
	~EntityManager();

	template <typename T>
	void registerComponent();

	EntityHandle createEntity();

	// TODO:
	// void destroyEntity(EntityHandle entHandle);

	template <typename T, typename ... Args>
	void assignComponent(EntityHandle entHandle, Args ... args);

	template <typename T>
	void detachComponent(EntityHandle entHandle);

	template <typename T>
	T* getComponent(EntityHandle entHandle);

	template <typename T>
	bool hasComponent(EntityHandle entHandle, ComponentSet set = ComponentSet{});

	template <typename T, typename S, typename ... Other>
	bool hasComponent(EntityHandle entHandle, ComponentSet set = ComponentSet{});

	template <typename T>
	ComponentType getComponentType();

private:

	bool match(EntityHandle entHandle, ComponentSet set);

	template <typename T, typename ... Args>
	void createComponent(EntityHandle entHandle, Args ... args);

	template <typename T>
	void removeComponent(EntityHandle entHandle);

	template <typename T>
	ComponentPool<T>* getPool();

	EntityPtr getEntity(EntityHandle entHandle);

	ComponentTypeMap _typemap{};
	PoolMap _pools{};

	std::vector<Entity> _entities{};
};

template <typename T>
T* ComponentPool<T>::getComponent(EntityHandle entHandle)
{
	auto it = _components.find(entHandle);

	if (it == _components.end())
		return nullptr;

	return &it->second;
}

template <typename T>
template <typename ... Args>
void ComponentPool<T>::createComponent(EntityHandle entHandle, Args... args)
{
	_components.emplace(entHandle, T(std::forward<Args>(args)...));
}

template <typename T>
void ComponentPool<T>::removeComponent(EntityHandle entHandle)
{
	auto it = _components.find(entHandle);

	if (it == _components.end())
		return;

	_components.erase(it);
}

template <typename T>
ComponentType ComponentTypeMap::getTypeID()
{
	std::type_index index(typeid(T));

	ComponentHash hash = index.hash_code();

	auto it = _component_types.find(hash);

	if (it != _component_types.end())
	{
		return it->second;
	}

	return INVALID_TYPE;
}

template <typename T>
ComponentType ComponentTypeMap::createTypeID()
{
	std::type_index index(typeid(T));
	ComponentHash hash = index.hash_code();

	_component_types.emplace(hash, _currID);

	return _currID++;
}

template <typename T>
void EntityManager::registerComponent()
{
	// Generate a new ID for the component
	ComponentType id = _typemap.getTypeID<T>();

	if (id != _typemap.INVALID_TYPE)
	{
		// Already registered!
		return;
	}

	_pools.emplace(_typemap.createTypeID<T>(), new ComponentPool<T>{});
}

template <typename T, typename ... Args>
void EntityManager::assignComponent(EntityHandle entHandle, Args... args)
{
	EntityPtr ePtr = getEntity(entHandle);

	//EM_ASSERT(ePtr != nullptr, "Entity Pointer is null");

	ePtr->_components.set(_typemap.getTypeID<T>());

	createComponent<T>(entHandle, std::forward<Args>(args)...);
}

template <typename T>
void EntityManager::detachComponent(EntityHandle entHandle)
{
	EntityPtr ePtr = getEntity(entHandle);

	EM_ASSERT(ePtr != nullptr, "Entity Pointer is null");

		ePtr->_components.set(_typemap.getTypeID<T>(), false);

	removeComponent<T>(entHandle);
}

template <typename T>
T* EntityManager::getComponent(EntityHandle entHandle)
{
	ComponentPool<T>* pool = getPool<T>();

	return pool->getComponent(entHandle);
}

template <typename T>
bool EntityManager::hasComponent(EntityHandle entHandle, ComponentSet set)
{
	set.set(_typemap.getTypeID<T>());

	return match(entHandle, set);
}

template <typename T, typename S, typename ... Other>
bool EntityManager::hasComponent(EntityHandle entHandle, ComponentSet set)
{
	set.set(_typemap.getTypeID<T>());

	return hasComponent<S, Other...>(entHandle, set);
}

template <typename T>
ComponentType EntityManager::getComponentType()
{
	return _typemap.getTypeID<T>();
}

template <typename T, typename ... Args>
void EntityManager::createComponent(EntityHandle entHandle, Args... args)
{
	ComponentPool<T>* pool = getPool<T>();

	pool->createComponent(entHandle, std::forward<Args>(args)...);
}

template <typename T>
void EntityManager::removeComponent(EntityHandle entHandle)
{
	ComponentPool<T>* pool = getPool<T>();

	pool->removeComponent(entHandle);
}

template <typename T>
ComponentPool<T>* EntityManager::getPool()
{
	ComponentType id = _typemap.getTypeID<T>();

	auto it = _pools.find(id);

	if (it == _pools.end())
		return nullptr; // Error: Component not registered.

	ComponentPool<T>* pool = dynamic_cast<ComponentPool<T>*>(it->second);

	return pool;
}
