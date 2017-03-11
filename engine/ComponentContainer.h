/**
* @file	ComponentContainer.h
* @Author	Kevin Kjellén
* @date	2017-03-05
* @brief	Container for all the components for all entities
*/

#include <map>
#include <typeinfo>
#include <vector>

template <typename T>
class ComponentPool : public std::map<uint32_t, T> {};

#pragma once
class ComponentContainer
{
public:
	template <typename T>
	static void addComponent(uint32_t entHandle, T& compHandle);

	template <typename T>
	static T* getComponent(uint32_t entHandle);


private:

	template <typename T>
	static ComponentPool<T>& getComponentPool()
	{
		static ComponentPool<T> components;

		return components;
	}
	
};

template <typename T>
void ComponentContainer::addComponent(uint32_t entHandle, T& compHandle)
{
	getComponentPool<T>().emplace(entHandle, compHandle);
}

template <typename T>
T* ComponentContainer::getComponent(uint32_t entHandle)
{
	return &getComponentPool<T>()[entHandle];
}

