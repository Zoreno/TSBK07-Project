/**
 * @file	Timer.h
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Simple class for keeping track of time
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief Simple timer
 */
class Timer
{
public:
	/**
	 * @brief Constructor. Initiates time to 0.
	 */
	Timer();

	/**
	 * @brief Resets the timer.
	 * @return Time before reset
	 */
	GLfloat reset();
private:

	/**
	 * @brief Current time.
	 */
	GLfloat time;
};