#include <iostream>

#include "uniform.h"
#include "shaderProgram.h"

// Family of functions setting vec2 uniforms

Uniform::Uniform() : _shaderProgram(nullptr), _location(-1)
{
}

Uniform::Uniform(const std::string& name, ShaderProgram* shaderProgram) :
    _name(name),
    _shaderProgram(shaderProgram)
{
    _location = glGetUniformLocation(_shaderProgram->getShaderProgramID(), _name.c_str());
    if (_location == -1)
    {
        std::cout << "WARNING: uniform with name " << name << " does not exist, setting it will fail!" << std::endl;
    }
}

Uniform& Uniform::operator=(const glm::vec2& vector2D)
{
    set(vector2D);
    return *this;
}

void Uniform::set(const glm::vec2& vector2D)
{
    glUniform2fv(_location, 1, (GLfloat*)&vector2D);
}

void Uniform::set(const glm::vec2* vectors2D, int count)
{
    glUniform2fv(_location, count, (GLfloat*)vectors2D);
}

// Family of functions setting vec3 uniforms

Uniform& Uniform::operator=(const glm::vec3& vector3D)
{
    set(vector3D);
    return *this;
}

void Uniform::set(const glm::vec3& vector3D)
{
    glUniform3fv(_location, 1, (GLfloat*)&vector3D);
}

void Uniform::set(const glm::vec3* vectors3D, int count)
{
    glUniform3fv(_location, count, (GLfloat*)vectors3D);
}

// Family of functions setting vec4 uniforms

Uniform& Uniform::operator=(const glm::vec4& vector4D)
{
    set(vector4D);
    return *this;
}

void Uniform::set(const glm::vec4& vector4D)
{
    glUniform4fv(_location, 1, (GLfloat*)&vector4D);
}

void Uniform::set(const glm::vec4* vectors4D, int count)
{
    glUniform4fv(_location, count, (GLfloat*)vectors4D);
}

// Family of functions setting float uniforms

Uniform& Uniform::operator=(const float floatValue)
{
    set(floatValue);
    return *this;
}

Uniform& Uniform::operator=(const std::vector<float>& floatValues)
{
    set(floatValues.data(), int(floatValues.size()));
    return *this;
}

void Uniform::set(const float floatValue)
{
    glUniform1fv(_location, 1, (GLfloat*)&floatValue);
}

void Uniform::set(const float* floatValues, int count)
{
    glUniform1fv(_location, count, (GLfloat*)floatValues);
}

// Family of functions setting integer uniforms

Uniform& Uniform::operator=(const int integerValue)
{
    set(integerValue);
    return *this;
}

void Uniform::set(const int integerValue)
{
    glUniform1iv(_location, 1, (GLint*)&integerValue);
}

void Uniform::set(const int* integerValues, int count)
{
    glUniform1iv(_location, count, (GLint*)integerValues);
}

Uniform& Uniform::operator=(const bool boolValue)
{
    set(boolValue);
    return *this;
}

void Uniform::set(const bool boolValue)
{
    set(boolValue ? 1 : 0);
}

void Uniform::set(const bool* boolValue, int count)
{
    std::vector<int> integerValues;
    for (auto i = 0; i < count; i++)
    {
        integerValues[i] = boolValue[i] ? 1 : 0;
    }

    set(integerValues.data(), count);
}

// Family of functions setting 3x3 matrices uniforms

Uniform& Uniform::operator=(const glm::mat3 & matrix)
{
    set(matrix);
    return *this;
}

void Uniform::set(const glm::mat3& matrix)
{
    glUniformMatrix3fv(_location, 1, false, (GLfloat*)&matrix);
}

void Uniform::set(const glm::mat3* matrices, int count)
{
    glUniformMatrix3fv(_location, count, false, (GLfloat*)matrices);
}

// Family of functions setting 4x4 matrices uniforms

Uniform& Uniform::operator=(const glm::mat4& matrix)
{
    set(matrix);
    return *this;
}

void Uniform::set(const glm::mat4& matrix)
{
    glUniformMatrix4fv(_location, 1, false, (GLfloat*)&matrix);
}

void Uniform::set(const glm::mat4* matrices, int count)
{
    glUniformMatrix4fv(_location, 1, false, (GLfloat*)matrices);
}