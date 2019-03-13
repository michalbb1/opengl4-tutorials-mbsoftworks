#pragma once

#include "../shaderProgram.h"

namespace shader_structs {

class ShaderStruct
{
public:
	virtual void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const = 0;

protected:
	static std::string constructAttributeName(const std::string& uniformName, const std::string& attributeName);
};

} // namespace shader_structs