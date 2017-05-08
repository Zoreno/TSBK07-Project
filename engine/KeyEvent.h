#pragma once

#include "Event.h"

class ENGINE_API KeyEvent : public Event
{
public:
	int key;
	int action;
};