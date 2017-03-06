/**
* @file	Entity.h
* @Author	Kevin Kjell�n
* @date	2017-03-06
* @brief	Entity-class holding all components
*/

#pragma once


#include <memory>
#include <map>
#include "Component.h"

// Denna beh�vs h�r av n�gon anledning?!?!
class Component;

class Entity
{
public:
	// Constructors, Destructors
	Entity();
	~Entity();

	// pub functions
	void update();

	// pub variables

private:
	// priv functions

	// priv variables
	std::map <uint32_t ,std::shared_ptr<Component>> Components;
};

