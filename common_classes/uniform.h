#pragma once

// STL
#include <vector>
#include <string>

// GLM
#include <glm/glm.hpp>

// GLAD
#include <glad/glad.h>

// Forward class declaration of ShaderProgram (because of cross-inclusion)
class ShaderProgram;

/**
 * Wraps OpenGL shader uniform variable.
 */
class Uniform
{
public:
    Uniform() = default; // Required to work with ShaderProgram [] operator
    Uniform(const std::string& name, ShaderProgram* shaderProgram);

    // Family of functions setting vec2 uniforms
    Uniform& operator=(const glm::vec2& vector2D);
    void set(const glm::vec2& vector2D) const;
    void set(const glm::vec2* vectors2D, GLsizei count = 1) const;

    // Family of functions setting vec3 uniforms
    Uniform& operator=(const glm::vec3& vector3D);
    void set(const glm::vec3& vector3D) const;
    void set(const glm::vec3* vectors3D, GLsizei count = 1) const;

    // Family of functions setting vec4 uniforms
    Uniform& operator=(const glm::vec4& vector4D);
    void set(const glm::vec4& vector4D) const;
    void set(const glm::vec4* vectors4D, GLsizei count = 1) const;

    // Family of functions setting float uniforms
    Uniform& operator=(GLfloat floatValue);
    Uniform& operator=(const std::vector<GLfloat>& floatValues);
    void set(GLfloat floatValue) const;
    void set(const GLfloat* floatValues, GLsizei count = 1) const;

    // Family of functions setting integer uniforms
    Uniform& operator=(GLint integerValue);
    void set(GLint integerValue) const;
    void set(const GLint* integerValues, GLsizei count = 1) const;

    // Family of functions setting 3x3 matrices uniforms
    Uniform& operator=(const glm::mat3& matrix);
    void set(const glm::mat3& matrix) const;
    void set(const glm::mat3* matrices, GLsizei count = 1) const;

    // Family of functions setting 4x4 matrices uniforms
    Uniform& operator=(const glm::mat4& matrix);
    void set(const glm::mat4& matrix) const;
    void set(const glm::mat4* matrices, GLsizei count = 1) const;

private:
    std::string name_; // Name of the uniform variable
    ShaderProgram* shaderProgram_{ nullptr }; // Pointer to shader program this uniform belongs to
    GLint location_{ -1 }; // OpenGL assigned uniform location (cached in this variable)
};
