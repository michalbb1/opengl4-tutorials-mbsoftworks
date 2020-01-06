#pragma once

#include "shaderStruct.h"

namespace shader_structs {

struct FogParameters : ShaderStruct
{
	static const int FOG_EQUATION_LINEAR;
	static const int FOG_EQUATION_EXP;
	static const int FOG_EQUATION_EXP2;

	FogParameters();
	FogParameters(const glm::vec3& color, float linearStart, float linearEnd, float density, int equation, const bool isEnabled = true);

	static const FogParameters& noFog();

	//* \brief Sets fog parameters in a shader program. */
	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	std::string getFogEquationName() const;

	glm::vec3 color; //!< Color to be used with fog, usually grayish
	float linearStart; //!< This is where linear fog starts (valid for linear equation only)
	float linearEnd; //!< This is where linear fog ends (valid for linear equation only)
	float density; //!< Density of the fog, used by exp and exp2 equation
	
	int equation; //!< Used fog equation
	bool isEnabled; //!< Flag telling if fog is enabled or not
};

} // namespace shader_structs