#pragma once

#include "shaderStruct.h"

namespace shader_structs {

class AmbientLight : ShaderStruct
{
public:
	AmbientLight(const glm::vec3& color, const bool isOn = true);

	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	glm::vec3 getColor() const;
	void setColor(const glm::vec3& color);

	bool isOn() const;
	void setOn(const bool isOn);

private:
	glm::vec3 _color;
	bool _isOn;
};

} // namespace shader_structs