#pragma once
#include "Component.h"
#include <rapidxml/rapidxml.hpp>
#include "EngineDLL.h"

class ENGINE_API CollisionComponent :
	public Component
{
public:
	CollisionComponent() = delete;
	explicit CollisionComponent(float reach) : reach{reach}{};
	explicit CollisionComponent(rapidxml::xml_node<>* node) : reach{} {}
	~CollisionComponent() = default;

	float getReach() const;
private:
	const float reach;
};

