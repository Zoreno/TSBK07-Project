/**
 * @file	Subscriber.h
 * @Author	Joakim Bertils
 * @date	2017-04-03
 * @brief	Base Class for an event subscriber.
 */

#pragma once

#include <type_traits>

#include "Event.h"

/**
 * @brief Subscriber base class.
 * @tparam T Event type.
 */
template <typename T>
class ENGINE_API Subscriber
{
public:

	/**
	 * @brief Destructor. 
	 */
	virtual ~Subscriber() {}

	/**
	 * @brief Pure virtual event handler.
	 * @param ev Const Ref. to event.
	 * @return Void.
	 */
	virtual
	typename std::enable_if<std::is_base_of<Event, T>::value>::type
		handleEvent(const T& ev) = 0;
};
