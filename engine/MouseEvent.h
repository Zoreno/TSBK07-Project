#pragma once
#include "Event.h"
#include <GL/glew.h>

class MouseEvent : public Event
{
public:
	GLfloat posX;
	GLfloat posY;
};
