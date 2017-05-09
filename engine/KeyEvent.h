/**
 * @file	KeyEvent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Key Event class
 */

#pragma once

#include "Event.h"

/**
 * @brief Key Event class
 */
class KeyEvent : public Event
{
public:

	/**
	 * @brief Keycode
	 */
	int key;

	/**
	 * @brief Key action (press/release)
	 */
	int action;
};