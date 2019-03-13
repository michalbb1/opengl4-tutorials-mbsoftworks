#pragma once

#include "shaderStruct.h"

namespace shader_structs {

class DiffuseLight : ShaderStruct
{
public:
	DiffuseLight(const glm::vec3& color, const glm::vec3& direction, const float factor, const bool isOn = true);

	void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const override;

	glm::vec3 getColor() const;
	void setColor(const glm::vec3& color);

	glm::vec3 getDirection() const;
	void setDirection(const glm::vec3& direction);

	float getFactor() const;
	void setFactor(const float factor);

	bool isOn() const;
	void setOn(const bool isOn);

private:
	glm::vec3 _color;
	glm::vec3 _direction;
	float _factor;
	bool _isOn;
};

} // namespace shader_structs