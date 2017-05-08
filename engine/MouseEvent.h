#pragma once
#include "Event.h"
#include <GL/glew.h>

class ENGINE_API MouseEvent : public Event
{
public:
	GLfloat posX;
	GLfloat posY;
};
