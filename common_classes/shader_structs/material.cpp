// Project
#include "material.h"

namespace shader_structs {

Material::Material(const float specularIntensity, const float specularPower, const bool isEnabled)
    : specularIntensity(specularIntensity)
    , specularPower(specularPower)
    , isEnabled(isEnabled)
{
}

const Material& Material::none()
{
    static Material noMaterial(0.0f, 0.0f, false);
    return noMaterial;
}

void Material::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
    shaderProgram[constructAttributeName(uniformName, "isEnabled")] = isEnabled;
    if (!isEnabled) {
        return; // Skip settings other parameters if material is not enabled
    }

    shaderProgram[constructAttributeName(uniformName, "specularIntensity")] = specularIntensity;
    shaderProgram[constructAttributeName(uniformName, "specularPower")] = specularPower;
}

} // namespace shader_structs