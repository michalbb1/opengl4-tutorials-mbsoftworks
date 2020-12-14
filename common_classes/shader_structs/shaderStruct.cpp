// Project
#include "shaderStruct.h"

namespace shader_structs {

std::string ShaderStruct::constructAttributeName(const std::string& uniformName, const std::string& attributeName)
{
    return uniformName + "." + attributeName;
}

} // namespace shader_structs