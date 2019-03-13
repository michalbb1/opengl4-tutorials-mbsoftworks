#include "ambientLight.h"

namespace shader_structs {

AmbientLight::AmbientLight(const glm::vec3& color, const bool isOn)
	: _color(color)
	, _isOn(isOn)
{
}

void AmbientLight::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
	shaderProgram[constructAttributeName(uniformName, "color")] = _color;
	shaderProgram[constructAttributeName(uniformName, "isOn")] = _isOn;
}

glm::vec3 AmbientLight::getColor() const
{
	return _color;
}

void AmbientLight::setColor(const glm::vec3& color)
{
	_color = color;
}

bool AmbientLight::isOn() const
{
	return _isOn;
}

void AmbientLight::setOn(const bool isOn)
{
	_isOn = isOn;
}

} // namespace shader_structs