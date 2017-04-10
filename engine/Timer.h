#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Timer
{
public:
	Timer();

	GLfloat reset();
private:
	GLfloat time;
};