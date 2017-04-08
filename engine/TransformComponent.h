#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include <sstream>
#include <rapidxml/rapidxml.hpp>

#include "Utils.h"

#include "TransformPipeline3D.h"

class TransformComponent : public Component
{
public:
	explicit TransformComponent(glm::vec3 position = glm::vec3{ 0.f,0.f,0.f }, float angle = 0.f, glm::vec3 rotationAxis = glm::vec3{ 0.f,1.f,0.f }, glm::vec3 scale = glm::vec3{ 1.f,1.f,1.f })
		: position(position), angle(angle), rotationAxis(rotationAxis), scale(scale) {}

	explicit TransformComponent(rapidxml::xml_node<>* node)
	{
		// Flummigt. Finns nog bättre sätt.

		std::stringstream ss1{ node->first_node("position")->value() };

		ss1 >> position;

		std::stringstream ss2{ node->first_node("angle")->value() };

		ss2 >> angle;

		std::stringstream ss3{ node->first_node("rotationAxis")->value() };

		ss3 >> rotationAxis;

		std::stringstream ss4{ node->first_node("scale")->value() };

		ss4 >> scale;
	}

	glm::vec3 position{};
	float angle{};
	glm::vec3 rotationAxis{};
	glm::vec3 scale{};

	TransformPipeline3D getPipeline() const
	{
		TransformPipeline3D pipeline{};

		pipeline.translate(position);

		if (angle != 0.f)
		{
			pipeline.rotate(angle, rotationAxis);
		}
		pipeline.scale(scale);

		return pipeline;
	}
};