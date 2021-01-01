// Project
#include "ambientLight.h"

namespace shader_structs {

AmbientLight::AmbientLight(const glm::vec3& color, const bool isOn)
    : color(color)
    , isOn(isOn)
{
}

void AmbientLight::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
    shaderProgram[constructAttributeName(uniformName, "color")] = color;
    shaderProgram[constructAttributeName(uniformName, "isOn")] = isOn;
}

glm::vec3 AmbientLight::getColorContribution() const
{
    return isOn ? color : glm::vec3(0.0f);
}

} // namespace shader_structs