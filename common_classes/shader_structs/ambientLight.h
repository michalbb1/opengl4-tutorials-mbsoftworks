#pragma once

#include "shaderStruct.h"

namespace shader_structs {

struct AmbientLight : ShaderStruct
{
	AmbientLight(const glm::vec3& color, const bool isOn = true);

	//* \brief Sets ambient light in a shader program. */
	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	/** \brief  Gets the final contributed color of this ambient light (depending if light is on or off).
	*   \return The ambient light color contribution.
	*/
	glm::vec3 getColorContribution() const;

	glm::vec3 color; //!< Color of the ambient light
	bool isOn; //!< Flag telling, if the light is on
};

} // namespace shader_structs