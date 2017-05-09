/**
 * @file	MouseEvent.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Mouse Event
 */

#pragma once

#include "Event.h"
#include <GL/glew.h>

/**
 * @brief Mouse Event class
 */
class MouseEvent : public Event
{
public:

	/**
	 * @brief Mouse x position
	 */
	GLfloat posX;

	/**
	 * @brief Mouse y position
	 */
	GLfloat posY;
};
