/**
* @file		EntityManager.h
* @Author	Joakim Bertils
* @date		2017-04-02
* @brief	Contains the Entity Component interface and implementation.
*/

#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <typeindex>
#include <memory>
#include <bitset>
#include <functional>

#include "Component.h"
#include "System.h"

// Bör var mer än tillräckligt
#define MAX_COMPONENTS 10

#define USE_ASSERT 1

#if USE_ASSERT == 1
#define EM_ASSERT(expr, message) if(!(expr)) {std::cerr << "ASSERTION FAILED: " << __FILE__ << ":" << __LINE__ << "[" << message << "]" << std::endl;}
#else
#define EM_ASSERT(expr, message)
#endif

//=============================================================================
// Interface
//=============================================================================

class EntityManager;

typedef std::bitset<MAX_COMPONENTS> ComponentSet;

typedef uint32_t EntityHandle;
typedef uint32_t ComponentType;
typedef size_t ComponentHash;

EntityHandle getNextHandle();

class Entity
{
	friend EntityManager;
public:
	// Constructors, Destructors
	Entity() : _components{}, _handle{ getNextHandle() } {}
	~Entity() {}

	Entity(const Entity& other) : _components(other._components), _handle{ other._handle } {}

	bool operator==(const Entity& other) const { return _handle == other._handle; }
	bool operator!=(const Entity& other) const { return _handle != other._handle; }

	EntityHandle getID() const { return _handle; }
private:
	ComponentSet _components;
	EntityHandle _handle;
};

typedef Entity* EntityPtr;

class BasePool
{
public:
	virtual ~BasePool() {}

	virtual void removeComponent(EntityHandle entHandle) = 0;
};

typedef std::map<ComponentType, BasePool*> PoolMap;

template <typename T>
class ComponentPool : public BasePool
{
public:
	ComponentPool() = default;
	~ComponentPool() override {}

	typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
		getComponent(EntityHandle entHandle);

	template <typename ... Args>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		createComponent(EntityHandle entHandle, Args ... args);

	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		removeComponent(EntityHandle entHandle) override;
private:

	std::map<EntityHandle, T> _components{};
};

class ComponentTypeMap
{
public:
	ComponentTypeMap() = default;
	~ComponentTypeMap() {}

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
		getTypeID() const;

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
		createTypeID();

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
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		registerComponent();

	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<System, T>::value>::type
		registerSystem(Args ... args);

	EntityHandle createEntity();
	void destroyEntity(EntityHandle entHandle);

	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		assignComponent(EntityHandle entHandle, Args ... args);

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		detachComponent(EntityHandle entHandle);

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
		getComponent(EntityHandle entHandle);

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type
		hasComponent(EntityHandle entHandle, ComponentSet set = ComponentSet{});

	template <typename T, typename S, typename ... Other>
	typename std::enable_if<std::is_base_of<Component, T>::value && std::is_base_of<Component, S>::value, bool>::type
		hasComponent(EntityHandle entHandle, ComponentSet set = ComponentSet{});

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
		getComponentType();

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, std::tuple<T*>>::type
		getComponents(EntityHandle entHandle);

	template <typename T, typename S, typename ... Other>
	typename std::enable_if<std::is_base_of<Component, T>::value && std::is_base_of<Component, S>::value, std::tuple<T*, S*, Other* ... >>::type
		getComponents(EntityHandle entHandle);

	template <typename ... Args>
	void each(typename std::identity<std::function<void(EntityHandle, Args*...)>>::type f);

	void update(float dt);

private:

	bool match(EntityHandle entHandle, ComponentSet set);

	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		createComponent(EntityHandle entHandle, Args ... args);

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		removeComponent(EntityHandle entHandle);

	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentPool<T>*>::type
		getPool();

	template <typename Func, typename Tup, std::size_t ... index>
	void invoke_helper(EntityHandle entHandle, Func&& func, Tup&& tup, std::index_sequence<index...>);

	template <typename Func, typename Tup>
	void invoke(EntityHandle entHandle, Func&& func, Tup&& tup);

	EntityPtr getEntity(EntityHandle entHandle);

	ComponentTypeMap _typemap{};
	PoolMap _pools{};
	std::vector<Entity> _entities{};
	std::vector<System*> _systems{};
};

//=============================================================================
// Implementation
//=============================================================================

//=============================================================================
// ComponentPool<T>
//=============================================================================

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
ComponentPool<T>::getComponent(EntityHandle entHandle)
{
	auto it = _components.find(entHandle);

	if (it == _components.end())
		return nullptr;

	return &it->second;
}

template <typename T>
template <typename ... Args>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
ComponentPool<T>::createComponent(EntityHandle entHandle, Args... args)
{
	_components.emplace(entHandle, T(std::forward<Args>(args)...));
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
ComponentPool<T>::removeComponent(EntityHandle entHandle)
{
	auto it = _components.find(entHandle);

	if (it == _components.end())
		return;

	_components.erase(it);
}

//=============================================================================
// ComponentTypeMap
//=============================================================================

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
ComponentTypeMap::getTypeID() const
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
typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
ComponentTypeMap::createTypeID()
{
	std::type_index index(typeid(T));
	ComponentHash hash = index.hash_code();

	_component_types.emplace(hash, _currID);

	return _currID++;
}

//=============================================================================
// EntityManager
//=============================================================================

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::registerComponent()
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
typename std::enable_if<std::is_base_of<System, T>::value>::type
EntityManager::registerSystem(Args ... args)
{
	T* system = new T(std::forward<Args...>(args)...);

	system->registerManager(this);

	_systems.push_back(system);
}

template <typename T, typename ... Args>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::assignComponent(EntityHandle entHandle, Args... args)
{
	EntityPtr ePtr = getEntity(entHandle);

	EM_ASSERT(ePtr != nullptr, "Entity Pointer is null");

	ePtr->_components.set(_typemap.getTypeID<T>());

	createComponent<T>(entHandle, std::forward<Args>(args)...);
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::detachComponent(EntityHandle entHandle)
{
	EntityPtr ePtr = getEntity(entHandle);

	EM_ASSERT(ePtr != nullptr, "Entity Pointer is null");

	ePtr->_components.set(_typemap.getTypeID<T>(), false);

	removeComponent<T>(entHandle);
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
EntityManager::getComponent(EntityHandle entHandle)
{
	ComponentPool<T>* pool = getPool<T>();

	return pool->getComponent(entHandle);
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type
EntityManager::hasComponent(EntityHandle entHandle, ComponentSet set)
{
	set.set(_typemap.getTypeID<T>());

	return match(entHandle, set);
}

template <typename T, typename S, typename ... Other>
typename std::enable_if<std::is_base_of<Component, T>::value && std::is_base_of<Component, S>::value, bool>::type
EntityManager::hasComponent(EntityHandle entHandle, ComponentSet set)
{
	set.set(_typemap.getTypeID<T>());

	return hasComponent<S, Other...>(entHandle, set);
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
EntityManager::getComponentType()
{
	return _typemap.getTypeID<T>();
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, std::tuple<T*>>::type
EntityManager::getComponents(EntityHandle entHandle)
{
	return std::make_tuple(getComponent<T>(entHandle));
}

template <typename T, typename S, typename ... Other>
typename std::enable_if<std::is_base_of<Component, T>::value && std::is_base_of<Component, S>::value, std::tuple<T*, S*, Other* ... >>::type
EntityManager::getComponents(EntityHandle entHandle)
{
	return std::tuple_cat(std::make_tuple(getComponent<T>(entHandle)), getComponents<S, Other...>(entHandle));
}

template <typename Func, typename Tup, std::size_t ... index>
void EntityManager::invoke_helper(EntityHandle entHandle, Func&& func, Tup&& tup, std::index_sequence<index...>)
{
	func(entHandle, std::get<index>(std::forward<Tup>(tup))...);
}

template <typename Func, typename Tup>
void EntityManager::invoke(EntityHandle entHandle, Func&& func, Tup&& tup)
{
	constexpr auto SIZE = std::tuple_size<typename std::decay<Tup>::type>::value;

	invoke_helper(entHandle, std::forward<Func>(func), std::forward<Tup>(tup), std::make_index_sequence<SIZE>{});
}

template <typename ... Args>
void EntityManager::each(typename std::identity<std::function<void(EntityHandle, Args*...)>>::type f)
{
	for (auto it : _entities)
	{
		if (hasComponent<Args...>(it.getID()))
		{
			invoke(it.getID(), f, getComponents<Args...>(it.getID()));
		}
	}
}

template <typename T, typename ... Args>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::createComponent(EntityHandle entHandle, Args... args)
{
	ComponentPool<T>* pool = getPool<T>();

	pool->createComponent(entHandle, std::forward<Args>(args)...);
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::removeComponent(EntityHandle entHandle)
{
	ComponentPool<T>* pool = getPool<T>();

	pool->removeComponent(entHandle);
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value, ComponentPool<T>*>::type
EntityManager::getPool()
{
	ComponentType id = _typemap.getTypeID<T>();

	auto it = _pools.find(id);

	if (it == _pools.end())
		return nullptr; // Error: Component not registered.

	ComponentPool<T>* pool = dynamic_cast<ComponentPool<T>*>(it->second);

	return pool;
}