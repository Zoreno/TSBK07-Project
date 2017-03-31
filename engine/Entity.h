/**
* @file	Entity.h
* @Author	Kevin Kjell�n
* @date	2017-03-06
* @brief	Entity-class holding all components
*/

#pragma once

#include <memory>
#include <map>
#include "EngineDLL.h"
#include <bitset>

class EntityManager;

// B�r var mer �n tillr�ckligt
#define MAX_COMPONENTS 10

typedef std::bitset<MAX_COMPONENTS> ComponentSet;

uint32_t getNextHandle();

class Entity
{
	friend EntityManager;
public:
	// Constructors, Destructors
	Entity() : _components{}, _handle{ getNextHandle() } { _components.none(); }
	~Entity();

	Entity(const Entity& other) : _components(other._components), _handle{other._handle} {}

	bool operator==(const Entity& other) const { return _handle == other._handle; }
	bool operator!=(const Entity& other) const { return _handle != other._handle; }

	uint32_t getID() const;
private:
	ComponentSet _components;

	uint32_t _handle;
};

typedef Entity* EntityPtr;