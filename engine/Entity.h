/**
* @file	Entity.h
* @Author	Kevin Kjellén
* @date	2017-03-06
* @brief	Entity-class holding all components
*/

#pragma once


#include <memory>
#include <map>
#include "EngineDLL.h"

class Component;

uint32_t getNextHandle();

class ENGINE_API Entity
{
public:
	// Constructors, Destructors
	Entity() = default;
	~Entity();

	// pub functions

	uint32_t getID() const;

	// pub variables

private:
	// priv functions

	// priv variables

	uint32_t handle{ getNextHandle() };
};

