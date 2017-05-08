#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "EngineDLL.h"

class ENGINE_API Timer
{
public:
	Timer();

	GLfloat reset();
private:
	GLfloat time;
};
