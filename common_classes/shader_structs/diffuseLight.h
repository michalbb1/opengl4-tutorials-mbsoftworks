#pragma once

#include "shaderStruct.h"

namespace shader_structs {

struct DiffuseLight : ShaderStruct
{
	DiffuseLight(const glm::vec3& color, const glm::vec3& direction, const float factor, const bool isOn = true);

	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	glm::vec3 color;
	glm::vec3 direction;
	float factor;
	bool isOn;
};

} // namespace shader_structs