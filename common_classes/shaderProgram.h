#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "shader.h"

/**
  Wraps OpenGL shader program creation and linking into a very convenient class.
*/

class ShaderProgram
{
public:
	void createProgram();
	bool addShaderToProgram(const Shader& shader);
	bool linkProgram();
	void useProgram();
	void deleteProgram();

	GLuint getShaderProgramID();

	// Setting vector uniforms
	void setUniform(const std::string& varName, const glm::vec2& vector2D);
	void setUniform(const std::string& varName, const glm::vec2* vectors2D, int count = 1);
	void setUniform(const std::string& varName, const glm::vec3& vector3D);
	void setUniform(const std::string& varName, const glm::vec3* vectors3D, int count = 1);
	void setUniform(const std::string& varName, const glm::vec4& vector4D);
	void setUniform(const std::string& varName, const glm::vec4* vectors4D, int count = 1);

	// Setting floats
	void setUniform(const std::string& varName, const float floatValue);
	void setUniform(const std::string& varName, const float* floatValues, int count = 1);

	// Setting integers
	void setUniform(const std::string& varName, const int integerValue);
	void setUniform(const std::string& varName, const int* integerValues, int count = 1);

	// Setting 3x3 matrices
	void setUniform(const std::string& varName, const glm::mat3& matrix);
	void setUniform(const std::string& varName, const glm::mat3* matrices, int count = 1);

	// Setting 4x4 matrices
	void setUniform(const std::string& varName, const glm::mat4& matrix);
	void setUniform(const std::string& varName, const glm::mat4* matrices, int count = 1);

	// Model and normal matrix setting ispretty common
	void setModelAndNormalMatrix(const std::string& modelMatrixName, const std::string& normalMatrixName, const glm::mat4& modelMatrix);

private:
	GLuint _shaderProgramID; // Shader program ID, assigned by OpenGL
	bool _isLinked = false; // Whether program has been linked and is ready to use
};
