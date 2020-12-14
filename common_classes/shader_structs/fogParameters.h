#pragma once

// Project
#include "shaderStruct.h"

namespace shader_structs {

/**
 * Represents fog parameters in a shader.
 */
struct FogParameters : ShaderStruct
{
    static const int FOG_EQUATION_LINEAR;
    static const int FOG_EQUATION_EXP;
    static const int FOG_EQUATION_EXP2;

    FogParameters();
    FogParameters(const glm::vec3& color, float linearStart, float linearEnd, float density, int equation, const bool isEnabled = true);

    /**
     * Fog parameters for using no fog (not enabled fog).
     */
    static const FogParameters& noFog();

    /**
     * Sets fog parameters in a shader program.
     *
     * @param shaderProgram  Shader program to set fog parameters in
     * @param uniformName    Name of the uniform variable
     */
    void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

    /**
     * Translates used fog equation code to human-readable string.
     */
    std::string getFogEquationName() const;

    glm::vec3 color; // Color to be used with fog, usually grayish
    float linearStart; // This is where linear fog starts (valid for linear equation only)
    float linearEnd; // This is where linear fog ends (valid for linear equation only)
    float density; // Density of the fog, used by exp and exp2 equation
    
    int equation; // Used fog equation
    bool isEnabled; // Flag telling if fog is enabled or not
};

} // namespace shader_structs