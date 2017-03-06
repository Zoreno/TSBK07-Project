/**
* @file	Component.h
* @Author	Kevin Kjellén
* @date	2017-03-06
* @brief	Abstract component base class
*/
#pragma once

#include <memory>
#include "EngineDLL.h"

class Entity;

class ENGINE_API Component
{
public:
	// Constructors, Destructors
	Component() = delete;
	explicit Component(std::shared_ptr<Entity> entHand);
	 ~Component();

	// pub functions
	virtual void update() = 0;

	// Include this in case of needed physics and such
	//virtual void fixedUpdate() = 0;

	// pub variables (should be none)
protected:
	// prot functions

	// prot variables
	std::shared_ptr<Entity> entityHandle;
private:
	// priv functions

	// priv variables
};

