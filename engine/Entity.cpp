/**
* @file	Entity.cpp
* @Author	Kevin Kjellén
* @date	2017-03-06
* @brief	Entity-class holding all components
*/

#include "Entity.h"
#include "Component.h"

Entity::Entity()
{
}


Entity::~Entity()
{
}

void Entity::update()
{
	for (auto i : Components) 
	{
		i.second->update();
	}
}
