#pragma once

#include "shaderStruct.h"

namespace shader_structs {

struct DiffuseLight : ShaderStruct
{
	DiffuseLight(const glm::vec3& color, const glm::vec3& direction, const float factor, const bool isOn = true);

	//* \brief Sets diffuse light in a shader program. */
	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	glm::vec3 color; //!< Color of the diffuse light
	glm::vec3 direction; //!< Direction of the diffuse light
	float factor; //!< Factor to multiply dot product with (strength of light)
	bool isOn; //!< Flag telling, if the light is on
};

} // namespace shader_structs