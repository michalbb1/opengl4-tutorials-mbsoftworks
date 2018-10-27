#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

/**
  Wraps OpenGL shader uniform variable
*/

class ShaderProgram;

class Uniform
{
public:
	Uniform();
	Uniform(const std::string& name, ShaderProgram* shaderProgram);

	// Family of functions setting vec2 uniforms
	Uniform& operator=(const glm::vec2& vector2D);
	void set(const glm::vec2& vector2D);
	void set(const glm::vec2* vectors2D, int count = 1);

	// Family of functions setting vec3 uniforms
	Uniform& operator=(const glm::vec3& vector3D);
	void set(const glm::vec3& vector3D);
	void set(const glm::vec3* vectors3D, int count = 1);

	// Family of functions setting vec4 uniforms
	Uniform& operator=(const glm::vec4& vector4D);
	void set(const glm::vec4& vector4D);
	void set(const glm::vec4* vectors4D, int count = 1);

	// Family of functions setting float uniforms
	Uniform& operator=(const float floatValue);
	void set(const float floatValue);
	void set(const float* floatValues, int count = 1);

	// Family of functions setting integer uniforms
	Uniform& operator=(const int integerValue);
	void set(const int integerValue);
	void set(const int* integerValues, int count = 1);

	// Family of functions setting 3x3 matrices uniforms
	Uniform& operator=(const glm::mat3& matrix);
	void set(const glm::mat3& matrix);
	void set(const glm::mat3* matrices, int count = 1);

	// Family of functions setting 4x4 matrices uniforms
	Uniform& operator=(const glm::mat4& matrix);
	void set(const glm::mat4& matrix);
	void set(const glm::mat4* matrices, int count = 1);

private:
	ShaderProgram* _shaderProgram; ///< Pointer to shader program this uniform belongs to

	std::string _name; ///< Name of uniform
	int _location; ///< OpenGL assigned uniform location (cached in this variable)
};
