#include "PointLight.h"

PointLight::PointLight(
	const glm::vec3& position,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	float constant,
	float linear,
	float quadratic)
	: position(position),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	constant(constant),
	linear(linear),
	quadratic(quadratic)
{}

glm::vec3 PointLight::getPosition() const
{
	return position;
}

float PointLight::getConstant() const
{
	return constant;
}

float PointLight::getLinear() const
{
	return linear;
}

float PointLight::getQuadratic() const
{
	return quadratic;
}

glm::vec3 PointLight::getAmbient() const
{
	return ambient;
}

glm::vec3 PointLight::getDiffuse() const
{
	return diffuse;
}

glm::vec3 PointLight::getSpecular() const
{
	return specular;
}
