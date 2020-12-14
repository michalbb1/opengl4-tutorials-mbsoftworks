// Project
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

const DiffuseLight& DiffuseLight::none()
{
    static DiffuseLight noneDiffuseLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, false);
    return noneDiffuseLight;
}

} // namespace shader_structs