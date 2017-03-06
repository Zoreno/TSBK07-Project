/**
* @file	Component.cpp
* @Author	Kevin Kjellén
* @date	2017-03-06
* @brief	Abstract component base class
*/

#include "Component.h"


Component::Component(std::shared_ptr<Entity> entHand) :
	entityHandle{entHand} 
{
}


Component::~Component() {
}
