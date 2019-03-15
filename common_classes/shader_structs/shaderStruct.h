#pragma once

#include "../shaderProgram.h"

namespace shader_structs {

struct ShaderStruct
{
public:

	/** \brief  Sets this shader structure as uniform variable.
	*   \param  shaderProgram Shader program to set uniform variable in
	*   \param  uniformName Name of the uniform variable
	*/
	virtual void setUniform(ShaderProgram& shaderProgram, const std::string& uniformName) const = 0;

protected:

	/** \brief  Constructs full uniform name containing of structure name and attribute name.
	*   \param  uniformName Name of the uniform variable
	*   \param  attributeName Name of the attribute in that structure
	*   \return Full uniform name.
	*/
	static std::string constructAttributeName(const std::string& uniformName, const std::string& attributeName);
};

} // namespace shader_structs