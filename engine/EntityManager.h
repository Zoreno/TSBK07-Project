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
#include <iterator>

#include <rapidxml/rapidxml.hpp>

#include "ComponentAssignedEvent.h"
#include "Component.h"
#include "System.h"
#include "Event.h"
#include "Subscriber.h"

// Bör var mer än tillräckligt
#define MAX_COMPONENTS 128

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

/**
 * @brief Type definition for the component bitset.
 */
typedef std::bitset<MAX_COMPONENTS> ComponentSet;

/**
 * @brief A handle which represents an entity.
 */
typedef uint32_t EntityHandle;

/**
 * @brief A representation of an invalid handle.
 */
const EntityHandle INVALID_ENTITY{ 0 };

/**
 * @brief An id referencing a registered component type.
 */
typedef uint32_t ComponentType;

/**
 * @brief A hash value of a component's type.
 */
typedef size_t ComponentHash;

/**
 * @brief Gets the next free handle which can be assigned to an entity.
 * @return Next free handle.
 */
EntityHandle getNextHandle();

/**
 * @brief Class to represent an entity.
 */
class ENGINE_API Entity
{
	friend EntityManager;
public:
	
	/**
	 * @brief Constructor.
	 */
	Entity() : _components{}, _handle{ getNextHandle() } {}

	/**
	 * @brief Destructor
	 */
	~Entity() {}

	/**
	 * @brief Copy constructor.
	 * @param other The entity to copy from.
	 */
	Entity(const Entity& other) : _components(other._components), _handle{ other._handle } {}

	/**
	 * @brief Equality operator
	 * @param other The entity to compare with.
	 * @return True if equal.
	 */
	bool operator==(const Entity& other) const { return _handle == other._handle; }

	/**
	* @brief Inequality operator
	* @param other The entity to compare with.
	* @return True if not equal.
	*/
	bool operator!=(const Entity& other) const { return _handle != other._handle; }

	/**
	 * @brief Gets the ID (Handle) of the Entity.
	 * @return Handle.
	 */
	EntityHandle getID() const { return _handle; }
private:
	/**
	 * @brief Bitset representing which components are associated with this entity.
	 */
	ComponentSet _components;

	/**
	 * @brief Entity Handle.
	 */
	EntityHandle _handle;
};

/**
 * @brief Entity pointer type definition.
 */
typedef Entity* EntityPtr;

/**
 * @brief Base class for the component pools
 */
class ENGINE_API BasePool
{
public:
	/**
	 * @brief Destructor
	 */
	virtual ~BasePool() {}

	/**
	 * @brief Removes the component associated with entity with handle entHandle.
	 * @param entHandle The entity handle.
	 */
	virtual void removeComponent(EntityHandle entHandle) = 0;

	/**
	* @brief Copies a component.
	* @param from Handle to src entity.
	* @param to Handle to dest entity.
	* @return Void.
	*/
	virtual void copyComponent(EntityHandle from, EntityHandle to) = 0;

	/**
	 * @brief Creates a component specified by a XML node.
	 * @param entHandle Entity Handle
	 * @param node Ptr to node.
	 */
	virtual void createComponentFromNode(EntityHandle entHandle, rapidxml::xml_node<>* node) = 0;
};

/**
 * @brief Type definition of a map containing all pools.
 */
typedef std::map<ComponentType, BasePool*> PoolMap;

/**
 * @brief Type representing a component storage pool.
 * @tparam T The Component to store.
 */
template <typename T>
class ComponentPool : public BasePool
{
public:
	/**
	 * @brief Constructor
	 */
	ComponentPool() = default;

	/**
	 * @brief Destructor. 
	 */
	~ComponentPool() override {}

	/**
	 * @brief Deleted Copy Constructor.
	 */
	ComponentPool(const ComponentPool&) = delete;

	/**
	 * @brief Deleted Move Constructor.
	 */
	ComponentPool(ComponentPool&&) = delete;

	/**
	 * @brief Deleted copy assignment.
	 * @return Ref to self.
	 */
	ComponentPool& operator=(const ComponentPool&) = delete;

	/**
	* @brief Deleted move assignment.
	* @return Ref to self.
	*/
	ComponentPool& operator=(ComponentPool&&) = delete;

	/**
	 * @brief Gets the component associated with entity.
	 * @param entHandle Handle to entity.
	 * @return Pointer to component.
	 */
	typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
		getComponent(EntityHandle entHandle);

	/**
	 * @brief Creates a component and associates it with the given entity.
	 * @tparam Args Types of arguments to forward to component construction.
	 * @param entHandle Handle to entity.
	 * @param args Arguments to forward to component construction
	 * @return Void.
	 */
	template <typename ... Args>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		createComponent(EntityHandle entHandle, Args ... args);

	/**
	* @brief Creates a component specified by a XML node.
	* @param entHandle Entity Handle
	* @param node Ptr to node.
	* @return Void.
	*/
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		createComponentFromNode(EntityHandle entHandle, rapidxml::xml_node<>* node) override;

	/**
	 * @brief Copies a component using the component's copy ctor.
	 * @param from Handle to src entity.
	 * @param to Handle to dest entity.
	 * @return Void.
	 */
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		copyComponent(EntityHandle from, EntityHandle to) override;

	/**
	 * @brief Removes a component associated with entity.
	 * @param entHandle Handle to entity
	 * @return Void.
	 */
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		removeComponent(EntityHandle entHandle) override;
private:
	/**
	 * @brief Map containing all components of type T.
	 */
	std::map<EntityHandle, T> _components{};
};

/**
 * @brief Class containing all type IDs for components.
 */
class ENGINE_API ComponentTypeMap
{
public:
	/**
	 * @brief Constructor.
	 */
	ComponentTypeMap() = default;

	/**
	 * @brief Destructor.
	 */
	~ComponentTypeMap() {}

	/**
	 * @brief Copy constructor
	 */
	ComponentTypeMap(const ComponentTypeMap&) = delete;

	/**
	 * @brief Move constructor
	 */
	ComponentTypeMap(ComponentTypeMap&&) = delete;

	/**
	 * @brief Copy assignment operator
	 * @return Ref to self.
	 */
	ComponentTypeMap& operator=(const ComponentTypeMap&) = delete;

	/**
	* @brief Move assignment operator
	* @return Ref to self.
	*/
	ComponentTypeMap& operator=(ComponentTypeMap&&) = delete;

	/**
	 * @brief Gets the type ID for the Component of type T.
	 * @tparam T Component type.
	 * @return INVALID_TYPE if not registered, otherwise component ID.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
		getTypeID() const;

	/**
	 * @brief Gets the type ID for the Component with string name.
	 * @param name Name of requested component type.
	 * @return Component type.
	 */
	ComponentType getTypeIDFromString(const std::string& name) const;

	/**
	 * @brief Creates a type ID for the component of type T.
	 * @tparam T Component type.
	 * @return ID of component.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
		createTypeID(const std::string& name);

	/**
	 * @brief ID representing an invalid type.
	 */
	const ComponentType INVALID_TYPE{ 0 };

private:
	/**
	 * @brief Next free ID.
	 */
	ComponentType _currID{ 1 };

	/**
	 * @brief Map storing the ID data.
	 */
	std::map<ComponentHash, ComponentType> _component_types{};

	/**
	 * @brief Map storing the ID data mapped with a string.
	 */
	std::map<std::string, ComponentType> _component_names{};
};

/**
 * @brief Class managing all entity-component based events.
 */
class ENGINE_API EventManager
{
	/**
	 * @brief Base class representing an internal event channel.
	 */
	class ENGINE_API InternalEventChannelBase
	{
	public:
		/**
		 * @brief Destructor.
		 */
		virtual ~InternalEventChannelBase() {}
	};

	/**
	 * @brief Class representing an internal event channel for the event of type T.
	 * @tparam T Type of event.
	 */
	template <typename T>
	class InternalEventChannel : public InternalEventChannelBase
	{
	public:

		/**
		 * @brief Constructor.
		 */
		InternalEventChannel() = default;

		/**
		 * @brief Destructor.
		 */
		~InternalEventChannel() override {}

		/**
		 * @brief Adds a new subscriber.
		 * @param sub Pointer to subscriber.
		 * @return Void.
		 */
		typename std::enable_if<std::is_base_of<Event, T>::value>::type
			addSubscriber(Subscriber<T>* sub);

		/**
		 * @brief Removes a subscriber.
		 * @param sub Pointer to subscriber.
		 * @return Void.
		 */
		typename std::enable_if<std::is_base_of<Event, T>::value>::type
			removeSubscriber(Subscriber<T>* sub);

		/**
		 * @brief Posts an event to all subscribers.
		 * @param ev Const Ref. to event.
		 * @return Void.
		 */
		typename std::enable_if<std::is_base_of<Event, T>::value>::type
			postEvent(const T& ev);
	private:

		/**
		 * @brief Vector containing all current subscribers.
		 */
		std::vector<Subscriber<T>*> _subscribers{};
	};

public:

	/**
	 * @brief Constructor.
	 */
	EventManager() = default;

	/**
	 * @brief Destructor.
	 */
	~EventManager();

	/**
	 * @brief Adds a subscriber to the channel handling type T.
	 * @tparam T Event Type.
	 * @param sub Pointer to subscriber.
	 * @return Void.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Event, T>::value>::type
		addSubscriber(Subscriber<T>* sub);

	/**
	* @brief Removes a subscriber from the channel handling type T.
	* @tparam T Event Type.
	* @param sub Pointer to subscriber.
	* @return Void.
	*/
	template <typename T>
	typename std::enable_if<std::is_base_of<Event, T>::value>::type
		removeSubscriber(Subscriber<T>* sub);

	/**
	 * @brief Posts an event to all subscribers registered to channel handling event type T.
	 * @tparam T Event Type.
	 * @param ev Const Ref. to event.
	 * @return Void.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Event, T>::value>::type
		postEvent(const T& ev);
private:

	/**
	 * @brief Type to store the hash of an event type.
	 */
	typedef size_t EventHash;
	
	/**
	 * @brief Entry in channels map.
	 */
	using Entry = std::pair<EventHash, InternalEventChannelBase*>;

	/**
	 * @brief Gets the hash value of the type T.
	 * @tparam T Event type.
	 * @return Hash value.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Event, T>::value, EventHash>::type
		hash() const;

	/**
	 * @brief Gets the internal channel associated with event type T.
	 * @tparam T Event type.
	 * @return Pointer to channel.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Event, T>::value, InternalEventChannel<T>*>::type
		getInternalChannel();

	/**
	 * @brief Map storing all the channels.
	 */
	std::map<EventHash, InternalEventChannelBase*> _channels;
};

/**
 * @brief Entity Manager Exception class.
 */
class ENGINE_API EntityManagerException : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};

/**
 * @brief Manager for all entities, components and systems.
 */
class ENGINE_API EntityManager
{
public:

	/**
	 * @brief Constructor.
	 */
	EntityManager() = delete;

	/**
	* @brief Copy Constructor.
	*/
	EntityManager(const EntityManager&) = delete;

	/**
	* @brief Move Constructor.
	*/
	EntityManager(EntityManager&&) = delete;

	/**
	* @brief Copy assignment operator.
	*/
	EntityManager& operator=(const EntityManager&) = delete;
	
	/**
	* @brief Move assignment operator.
	*/
	EntityManager& operator=(EntityManager&&) = delete;

	/**
	 * @brief Constructor.
	 * @param ev Pointer to a valid event manager.
	 * @param am Pointer to a valid asset manager.
	 * @param ui Pointer to a vaild UserInterface manager
	 */
	explicit EntityManager(EventManager* ev, AssetManager* am, userinterface::UIManager* ui) : eventManager{ ev }, assetManager{ am }, uiManager{ ui } {}

	/**
	 * @brief Destructor.
	 */
	~EntityManager();

	/**
	 * @brief Registers a component type.
	 * 
	 * This will generate an ID for the component and allocate a storage pool.
	 * 
	 * @tparam T Component Type.
	 * @return Void.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		registerComponent(const std::string& name);

	/**
	 * @brief Registers a system.
	 * 
	 * This will register and instantiate a system.
	 * 
	 * @tparam T System type.
	 * @tparam Args Type of arguments to forward to construction.
	 * @param args Arguments to forward to construction.
	 * @return Void.
	 */
	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<System, T>::value>::type
		registerSystem(Args ... args);

	/**
	 * @brief Creates a new blank entity.
	 * @return Entity Handle.
	 */
	EntityHandle createEntity();

	/**
	 * @brief Copies an entity.
	 * @param from Entiity handle of original.
	 * @return Entity handle of new entity.
	 */
	EntityHandle copyEntity(EntityHandle from);

	/**
	 * @brief Creates an entity from a file.
	 * @param filePath Path to file.
	 * @return Entity Handle.
	 */
	EntityHandle createEntityFromFile(const char* filePath);

	/**
	 * @brief Destroys an entity and all associated components.
	 * @param entHandle Entity Handle.
	 */
	void destroyEntity(EntityHandle entHandle);

	/**
	 * @brief Assigns a component to an existing entity.
	 * @tparam T Component Type.
	 * @tparam Args Type of arguments to forward to construction of component.
	 * @param entHandle Entity Handle.
	 * @param args Argument to forward to construction of component.
	 * @return Void.
	 */
	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		assignComponent(EntityHandle entHandle, Args ... args);

	/**
	 * @brief Detaches (and destroys) component of type T associated with entity.
	 * @tparam T Component type.
	 * @param entHandle Entity handle.
	 * @return Void.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		detachComponent(EntityHandle entHandle);

	/**
	 * @brief Gets the component of type T associated with entity.
	 * @tparam T Component type.
	 * @param entHandle Entity Handle
	 * @return Pointer to component. Nullptr if entity is not associated with component of type T.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
		getComponent(EntityHandle entHandle);

	/**
	 * @brief Checks whether entity has a component of type T.
	 * @tparam T Component type.
	 * @param entHandle Entity Handle.
	 * @param set Component set. Only for internal use.
	 * @return True if entity is associated with component of type T.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type
		hasComponent(EntityHandle entHandle, ComponentSet set = ComponentSet{});

	/**
	 * @brief Checks whether entity has components of type T, S, Other...
	 * 
	 * This will be unpacked to check if entity is associated with all 
	 * component types passed in.
	 * 
	 * @tparam T First component type.
	 * @tparam S Second component type.
	 * @tparam Other Other component types.
	 * @param entHandle Entity Handle.
	 * @param set Component set. Only for internal use.
	 * @return True if entity is associated with all component types.
	 */
	template <typename T, typename S, typename ... Other>
	typename std::enable_if<std::is_base_of<Component, T>::value && std::is_base_of<Component, S>::value, bool>::type
		hasComponent(EntityHandle entHandle, ComponentSet set = ComponentSet{});

	/**
	 * @brief Gets the component ID associated with type T.
	 * @tparam T Component type.
	 * @return ID of component or @ComponentTypeMap::INVALID_VALUE if invalid.
	 * @see @ComponentTypeMap::INVALID_VALUE
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentType>::type
		getComponentType();

	/**
	 * @brief Get the component of type T associated with entity packed into a tuple.
	 * @tparam T Component type.
	 * @param entHandle Entity Handle.
	 * @return Pointer to component packed into a tuple.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, std::tuple<T*>>::type
		getComponents(EntityHandle entHandle);

	/**
	 * @brief Gets the component of type T, S, Other...  associated with entity packed into a tuple.
	 * @tparam T First component type.
	 * @tparam S Second component type.
	 * @tparam Other Other component types.
	 * @param entHandle Entity Handle.
	 * @return Pointers to components packed into a tuple.
	 */
	template <typename T, typename S, typename ... Other>
	typename std::enable_if<std::is_base_of<Component, T>::value && std::is_base_of<Component, S>::value, std::tuple<T*, S*, Other* ... >>::type
		getComponents(EntityHandle entHandle);

	/**
	 * @brief Applies a function on all entities associated with all componentof types Args.
	 * 
	 * The function signature must match the following:
	 * 
	 * EntityHandle
	 * Pointer to Component Types in order.
	 * 
	 * This function works with (given that the signatures match):
	 * 
	 * operator()
	 * 
	 * Lambda functions. (use [this](){} to create a member lambda.)
	 * 
	 * static functions.
	 * 
	 * @tparam Args Component types to match. 
	 * @param f Function to apply.
	 */
	template <typename ... Args>
	void each(typename std::identity<std::function<void(EntityHandle, Args*...)>>::type f);

	/**
	* @brief Gets an entity associated with handle.
	* @param entHandle Entity Handle.
	* @return Pointer to entity associated with handle.
	*/
	EntityPtr getEntity(EntityHandle entHandle);

	/**
	 * @brief Run a update step accounting for time dt(in seconds.s)
	 * @param dt Step time.
	 */
	void update(float dt);
private:

	/**
	 * @brief Checks whether a component has all components specified in set.
	 * @param entHandle Entity Handle.
	 * @param set Component set.
	 * @return True if entity is associated with all component in set.
	 */
	bool match(EntityHandle entHandle, ComponentSet set);

	/**
	 * @brief Creates a component of type T and associates it with entity.
	 * @tparam T Type of entity.
	 * @tparam Args Type of argument to forward to construction.
	 * @param entHandle Handle to entity.
	 * @param args Arguments to forward to construction.
	 * @return Void.
	 */
	template <typename T, typename ... Args>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		createComponent(EntityHandle entHandle, Args ... args);

	/**
	 * @brief Removes a component of type T associated with entity.
	 * @tparam T Component type.
	 * @param entHandle Entity Handle.
	 * @return Void.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value>::type
		removeComponent(EntityHandle entHandle);

	/**
	 * @brief Gets the Component storage pool associated with type T.
	 * @tparam T Component type.
	 * @return Pointer to component pool.
	 */
	template <typename T>
	typename std::enable_if<std::is_base_of<Component, T>::value, ComponentPool<T>*>::type
		getPool();

	/**
	 * @brief Helper to invoke function.
	 * @tparam Func Function type.
	 * @tparam Tup Tuple type.
	 * @tparam index Indices to unpack arguments from tuple.
	 * @param entHandle Entity Handle.
	 * @param func Function to be called.
	 * @param tup Tuple containing arguments to function.
	 * @param seq Integer sequence to help unpack argument.
	 */
	template <typename Func, typename Tup, std::size_t ... index>
	void invoke_helper(EntityHandle entHandle, Func&& func, Tup&& tup, std::index_sequence<index...> seq);

	/**
	 * @brief Helper to invoke function.
	 * @tparam Func Function type.
	 * @tparam Tup Tuple type.
	 * @param entHandle Entity Handle.
	 * @param func Function to be called.
	 * @param tup Tuple containing arguments to function. 
	 */
	template <typename Func, typename Tup>
	void invoke(EntityHandle entHandle, Func&& func, Tup&& tup);

	/**
	 * @brief Adds and removes pending entities.
	 */
	void refresh();

	/**
	 * @brief Storage for the type IDs.
	 */
	ComponentTypeMap _typemap{};

	/**
	 * @brief Storage for all component pools.
	 */
	PoolMap _pools{};

	/**
	 * @brief Storage for all entities in contiguous memory.
	 */
	std::vector<Entity> _entities{};

	/**
	 * @brief Storage for all entities waiting to be added.
	 */
	std::vector<Entity> _ent_to_add{};

	/**
	* @brief Storage for all entities waiting to be removed.
	*/
	std::vector<EntityHandle> _ent_to_remove{};

	/**
	 * @brief Storage for all systems.
	 */
	std::vector<System*> _systems{};

	/**
	 * @brief Pointer to event manager.
	 */
	EventManager* eventManager;

	/**
	 * @brief Pointer to asset manager
	 */
	AssetManager* assetManager;

	/**
	 * @brief Pointer to UserInterface manager.
	 */
	userinterface::UIManager* uiManager;
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
ComponentPool<T>::createComponentFromNode(EntityHandle entHandle, rapidxml::xml_node<>* node)
{
	// TODO: Error checking
	_components.emplace(entHandle, T(node));
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type 
ComponentPool<T>::copyComponent(EntityHandle from, EntityHandle to)
{
	T* orig = getComponent(from);

	_components.emplace(to, T(*orig));
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
ComponentTypeMap::createTypeID(const std::string& name)
{
	std::type_index index(typeid(T));
	ComponentHash hash = index.hash_code();

	_component_types.emplace(hash, _currID);
	_component_names.emplace(name, _currID);

	return _currID++;
}

//=============================================================================
// EventManager
//=============================================================================

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value>::type
EventManager::InternalEventChannel<T>::addSubscriber(Subscriber<T>* sub)
{
	_subscribers.push_back(sub);
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value>::type
EventManager::InternalEventChannel<T>::removeSubscriber(Subscriber<T>* sub)
{
	for (auto it = _subscribers.begin(); it != _subscribers.end(); ++it)
	{
		if (*it == sub)
		{
			_subscribers.erase(it);
			break;
		}
	}
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value>::type
EventManager::InternalEventChannel<T>::postEvent(const T& ev)
{
	for (auto it : _subscribers)
	{
		it->handleEvent(ev);
	}
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value>::type
EventManager::addSubscriber(Subscriber<T>* sub)
{
	getInternalChannel<T>()->addSubscriber(sub);
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value>::type 
EventManager::removeSubscriber(Subscriber<T>* sub)
{
	getInternalChannel<T>()->removeSubscriber(sub);
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value>::type
EventManager::postEvent(const T& ev)
{
	getInternalChannel<T>()->postEvent(ev);
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value, EventManager::EventHash>::type
EventManager::hash() const
{
	return std::type_index{ typeid(T) }.hash_code();
}

template <typename T>
typename std::enable_if<std::is_base_of<Event, T>::value, EventManager::InternalEventChannel<T>*>::type
EventManager::getInternalChannel()
{
	std::map<EventHash, InternalEventChannelBase*>::iterator it = _channels.find(hash<T>());

	if (it == _channels.end())
	{
		it = _channels.insert(Entry(hash<T>(), new InternalEventChannel<T>())).first;
	}

	return dynamic_cast<InternalEventChannel<T>*>(it->second);
}

//=============================================================================
// EntityManager
//=============================================================================

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::registerComponent(const std::string& name)
{
	// Generate a new ID for the component
	ComponentType id = _typemap.getTypeID<T>();

	if (id != _typemap.INVALID_TYPE)
		throw EntityManagerException{ "Component is already registered." };

	_pools.emplace(_typemap.createTypeID<T>(name), new ComponentPool<T>{});
}

template <typename T, typename ... Args>
typename std::enable_if<std::is_base_of<System, T>::value>::type
EntityManager::registerSystem(Args ... args)
{
	T* system = new T(std::forward<Args...>(args)...);

	system->registerManagers(this, eventManager, assetManager, uiManager);

	_systems.push_back(system);

	system->startUp();
}

template <typename T, typename ... Args>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::assignComponent(EntityHandle entHandle, Args... args)
{
	EntityPtr ePtr = getEntity(entHandle);

	if (!ePtr)
		throw EntityManagerException{ "Assigning component to invalid entity." };

	ePtr->_components.set(_typemap.getTypeID<T>());

	createComponent<T>(entHandle, std::forward<Args>(args)...);

	eventManager->postEvent(ComponentAssignedEvent<T>(entHandle));
}

template <typename T>
typename std::enable_if<std::is_base_of<Component, T>::value>::type
EntityManager::detachComponent(EntityHandle entHandle)
{
	EntityPtr ePtr = getEntity(entHandle);

	if (!ePtr)
		throw EntityManagerException{ "Detaching component from invalid entity." };

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
void EntityManager::invoke_helper(EntityHandle entHandle, Func&& func, Tup&& tup, std::index_sequence<index...> seq)
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
		throw EntityManagerException{ "Component is not yet registered." };

	ComponentPool<T>* pool = dynamic_cast<ComponentPool<T>*>(it->second);

	return pool;
}