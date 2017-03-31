/**
* @file	Entity.cpp
* @Author	Kevin Kjellén
* @date	2017-03-06
* @brief	Entity-class holding all components
*/

#include "Entity.h"
#include "Component.h"

uint32_t getNextHandle()
{
	static uint32_t next = 0;

	return next++;
}

Entity::~Entity()
{
}

uint32_t Entity::getID() const
{
	return _handle;
}
