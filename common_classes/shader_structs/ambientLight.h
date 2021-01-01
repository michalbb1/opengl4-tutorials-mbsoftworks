#pragma once

// Project
#include "shaderStruct.h"

namespace shader_structs {

/**
 * Represents ambient light in a shader.
 */
struct AmbientLight : ShaderStruct
{
    AmbientLight(const glm::vec3& color, const bool isOn = true);

    /**
     * Sets ambient light in a shader program.
     */
    void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

    /**
     * Gets the final contributed color of this ambient light (depending if light is on or off).
     * The ambient light color contribution.
     */
    glm::vec3 getColorContribution() const;

    glm::vec3 color; // Color of the ambient light
    bool isOn; // Flag telling, if the light is on
};

} // namespace shader_structs