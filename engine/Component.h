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
	Component() {}
	virtual ~Component();

	// pub functions

	// Include this in case of needed physics and such
	//virtual void fixedUpdate() = 0;

	// pub variables (should be none)
protected:
	// prot functions

	// prot variables
private:
	// priv functions

	// priv variables
};

