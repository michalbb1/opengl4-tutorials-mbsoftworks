#include "diffuseLight.h"

namespace shader_structs {

DiffuseLight::DiffuseLight(const glm::vec3& color, const glm::vec3& direction, const float factor, const bool isOn)
	: _color(color)
	, _direction(direction)
	, _factor(factor)
	, _isOn(isOn)
{
}

void DiffuseLight::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
	shaderProgram[constructAttributeName(uniformName, "color")] = _color;
	shaderProgram[constructAttributeName(uniformName, "direction")] = _direction;
	shaderProgram[constructAttributeName(uniformName, "factor")] = _factor;
	shaderProgram[constructAttributeName(uniformName, "isOn")] = _isOn;
}

glm::vec3 DiffuseLight::getColor() const
{
	return _color;
}

void DiffuseLight::setColor(const glm::vec3& color)
{
	_color = color;
}

glm::vec3 DiffuseLight::getDirection() const
{
	return _direction;
}

void DiffuseLight::setDirection(const glm::vec3& direction)
{
	_direction = direction;
}

float DiffuseLight::getFactor() const
{
	return _factor;
}

void DiffuseLight::setFactor(const float factor)
{
	_factor = factor;
}

bool DiffuseLight::isOn() const
{
	return _isOn;
}

void DiffuseLight::setOn(const bool isOn)
{
	_isOn = isOn;
}

} // namespace shader_structs