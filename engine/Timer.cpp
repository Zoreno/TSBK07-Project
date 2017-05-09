/**
 * @file	Timer.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Simple timer class
 */

#include "Timer.h"

Timer::Timer()
	: time{ static_cast<GLfloat>(glfwGetTime()) } {}

GLfloat Timer::reset()
{
	GLfloat oldTime = time;
	time = static_cast<GLfloat>(glfwGetTime());

	return (time - oldTime);
}
