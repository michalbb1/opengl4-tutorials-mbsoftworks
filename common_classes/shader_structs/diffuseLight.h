#pragma once

// Project
#include "shaderStruct.h"

namespace shader_structs {

/**
 * Represents diffuse light in a shader.
 */
struct DiffuseLight : ShaderStruct
{
    DiffuseLight(const glm::vec3& color, const glm::vec3& direction, const float factor, const bool isOn = true);

    /**
     * Sets diffuse light in a shader program.
     */
    void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

    /**
     * Returns a diffuse light that is off and does not contribute at all.
     */
    static const DiffuseLight& none();

    glm::vec3 color; // Color of the diffuse light
    glm::vec3 direction; // Direction of the diffuse light
    float factor; // Factor to multiply dot product with (strength of light)
    bool isOn; // Flag telling, if the light is on
};

} // namespace shader_structs