#pragma once

#include "shaderStruct.h"

namespace shader_structs {

struct Material : ShaderStruct
{
	Material(const float specularIntensity, const float specularPower, const bool isEnabled = true);

	//* \brief Material that does no calculations (is not enabled). */
	static const Material& none();

	//* \brief Sets material structure in a shader program. */
	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	bool isEnabled; //!< Flag telling if calculations with material are enabled
	float specularIntensity; //!< Factor to multiply specular highlight by
	float specularPower; //!< Number to raise calculated specular factor to
};

} // namespace shader_structs