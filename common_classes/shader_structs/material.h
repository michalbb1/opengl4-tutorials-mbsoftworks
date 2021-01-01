#pragma once

// Project
#include "shaderStruct.h"

namespace shader_structs {

/**
 * Represents material in a shader.
 */
struct Material : ShaderStruct
{
    Material(const float specularIntensity, const float specularPower, const bool isEnabled = true);

    /**
     * Material that does no calculations (is not enabled).
     */
    static const Material& none();

    /**
     * Sets material structure in a shader program.
     *
     * @param shaderProgram  Shader program to set material in
     * @param uniformName    Name of the uniform variable
     */
    void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

    bool isEnabled; // Flag telling if calculations with material are enabled
    float specularIntensity; // Factor to multiply specular highlight by
    float specularPower; // Number to raise calculated specular factor to
};

} // namespace shader_structs