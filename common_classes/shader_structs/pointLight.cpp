// Project
#include "pointLight.h"

namespace shader_structs {

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, const float ambientFactor,
    const float constantAttenuation, const float linearAttenuation, const float exponentialAttenuation,
    const bool isOn)
    : position(position)
    , color(color)
    , ambientFactor(ambientFactor)
    , constantAttenuation(constantAttenuation)
    , linearAttenuation(linearAttenuation)
    , exponentialAttenuation(exponentialAttenuation)
    , isOn(isOn ? 1 : 0)
{
}

void PointLight::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
    shaderProgram[constructAttributeName(uniformName, "position")] = position;
    shaderProgram[constructAttributeName(uniformName, "color")] = color;
    shaderProgram[constructAttributeName(uniformName, "ambientFactor")] = ambientFactor;
    shaderProgram[constructAttributeName(uniformName, "constantAttenuation")] = constantAttenuation;
    shaderProgram[constructAttributeName(uniformName, "linearAttenuation")] = linearAttenuation;
    shaderProgram[constructAttributeName(uniformName, "exponentialAttenuation")] = exponentialAttenuation;
    shaderProgram[constructAttributeName(uniformName, "isOn")] = isOn;
}

GLsizeiptr PointLight::getDataSizeStd140()
{
    // Here it's three times size of vec4 because:
    // - position + dummy padding make first vec4
    // - color + ambient factor make second vec4
    // - attenuation factors + isOn make third vec4
    return sizeof(glm::vec4) * 3;
}

void* PointLight::getDataPointer() const
{
    return (void*)&position;
}

const PointLight& PointLight::none()
{
    static PointLight nonePointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f, 0.0f, false);
    return nonePointLight;
}

} // namespace shader_structs