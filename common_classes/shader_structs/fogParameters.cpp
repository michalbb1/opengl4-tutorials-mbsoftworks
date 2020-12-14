// Project
#include "fogParameters.h"

namespace shader_structs {

const int FogParameters::FOG_EQUATION_LINEAR = 0;
const int FogParameters::FOG_EQUATION_EXP = 1;
const int FogParameters::FOG_EQUATION_EXP2 = 2;

FogParameters::FogParameters()
    : isEnabled(false)
{
}

FogParameters::FogParameters(const glm::vec3& color, float linearStart, float linearEnd, float density, int equation, const bool isEnabled)
    : color(color)
    , linearStart(linearStart)
    , linearEnd(linearEnd)
    , density(density)
    , equation(equation)
    , isEnabled(isEnabled)
{
}

const FogParameters& FogParameters::noFog()
{
    static FogParameters noFogParameters;
    return noFogParameters;
}

void FogParameters::setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const
{
    shaderProgram[constructAttributeName(uniformName, "isEnabled")] = isEnabled;
    if (!isEnabled) {
        return; // Skip settings other parameters if fog is not enabled
    }

    shaderProgram[constructAttributeName(uniformName, "color")] = color;
    shaderProgram[constructAttributeName(uniformName, "linearStart")] = linearStart;
    shaderProgram[constructAttributeName(uniformName, "linearEnd")] = linearEnd;
    shaderProgram[constructAttributeName(uniformName, "density")] = density;
    shaderProgram[constructAttributeName(uniformName, "equation")] = equation;
}

std::string FogParameters::getFogEquationName() const
{
    return equation == FOG_EQUATION_LINEAR ? "Linear" : equation == FOG_EQUATION_EXP ? "Exp" : "Exp2";
}

} // namespace shader_structs