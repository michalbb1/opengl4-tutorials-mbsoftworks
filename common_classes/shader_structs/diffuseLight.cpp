#include "diffuseLight.h"

namespace shader_structs {

DiffuseLight::DiffuseLight(const glm::vec3& color, const glm::vec3& direction, const float factor, const bool isOn)
	: color(color)
	, direction(direction)
	, factor(factor)
	, isOn(isOn)
{
}

void DiffuseLight::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
	shaderProgram[constructAttributeName(uniformName, "color")] = color;
	shaderProgram[constructAttributeName(uniformName, "direction")] = direction;
	shaderProgram[constructAttributeName(uniformName, "factor")] = factor;
	shaderProgram[constructAttributeName(uniformName, "isOn")] = isOn;
}

} // namespace shader_structs