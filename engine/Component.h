/**
* @file	Component.h
* @Author	Kevin Kjellén
* @date	2017-03-06
* @brief	Abstract component base class
*/
#pragma once

#include <memory>
#include "Entity.h"

//Denna behövs här av någon anledning?!?!
class Entity;

class Component
{
public:
	// Constructors, Destructors
	Component() = delete;
	explicit Component(std::shared_ptr<Entity> entHand);
	 ~Component();

	// pub functions
	virtual void update() = 0;

	// pub variables (should be none)
protected:
	// prot functions

	// prot variables
	std::shared_ptr<Entity> entityHandle;
private:
	// priv functions

	// priv variables
};

