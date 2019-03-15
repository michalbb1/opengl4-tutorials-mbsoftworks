#pragma once

#include "shaderStruct.h"

namespace shader_structs {

struct AmbientLight : ShaderStruct
{
	AmbientLight(const glm::vec3& color, const bool isOn = true);

	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;
	glm::vec3 getColorContribution() const;

	glm::vec3 color;
	bool isOn;
};

} // namespace shader_structs