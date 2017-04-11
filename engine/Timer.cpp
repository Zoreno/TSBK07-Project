#include "Timer.h"

Timer::Timer()
	: time{ static_cast<GLfloat>(glfwGetTime()) } {}

GLfloat Timer::reset()
{
	GLfloat oldTime = time;
	time = static_cast<GLfloat>(glfwGetTime());

	return (time - oldTime);
}
