#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <string>

#include "shader.h"

/**
  Wraps OpenGL shader program creation and linking into a very convenient class.
*/

class ShaderProgram
{
public:

	//* \brief Creates a new shader program.
	void createProgram();

	/** \brief Adds a shader to shader program. Shader must be properly loaded and compiled.
	*   \return True, if the shader has been added or false otherwise.
	*/
	bool addShaderToProgram(const Shader& shader);

	/** \brief Links the program. If the function succeeds, shader program is ready to use.
	*   \return True, if the shader has been linked or false otherwise.
	*/
	bool linkProgram();

	//* \brief Uses this shader program (makes current).
	void useProgram();

	//* \brief Deletes this shader program.
	void deleteProgram();

	/** \brief Gets OpenGL-assigned shader program ID.
	*   \return Shader program ID.
	*/
	GLuint getShaderProgramID();

	// Family of functions setting vec2 uniforms
	void setUniform(const std::string& varName, const glm::vec2& vector2D);
	void setUniform(const std::string& varName, const glm::vec2* vectors2D, int count = 1);

	// Family of functions setting vec3 uniforms
	void setUniform(const std::string& varName, const glm::vec3& vector3D);
	void setUniform(const std::string& varName, const glm::vec3* vectors3D, int count = 1);

	// Family of functions setting vec4 uniforms
	void setUniform(const std::string& varName, const glm::vec4& vector4D);
	void setUniform(const std::string& varName, const glm::vec4* vectors4D, int count = 1);

	// Family of functions setting float uniforms
	void setUniform(const std::string& varName, const float floatValue);
	void setUniform(const std::string& varName, const float* floatValues, int count = 1);

	// Family of functions setting integer uniforms
	void setUniform(const std::string& varName, const int integerValue);
	void setUniform(const std::string& varName, const int* integerValues, int count = 1);

	// Family of functions setting 3x3 matrices uniforms
	void setUniform(const std::string& varName, const glm::mat3& matrix);
	void setUniform(const std::string& varName, const glm::mat3* matrices, int count = 1);

	// Family of functions setting 4x4 matrices uniforms
	void setUniform(const std::string& varName, const glm::mat4& matrix);
	void setUniform(const std::string& varName, const glm::mat4* matrices, int count = 1);

	// Model and normal matrix setting is pretty common, that's why this convenience function
	void setModelAndNormalMatrix(const std::string& modelMatrixName, const std::string& normalMatrixName, const glm::mat4& modelMatrix);

private:

	/** \brief Gets uniform variable location. If the location is cached, it is just retrieved, otherwise it's queried and cached.
	*   \param  varName uniform variable name
	*   \return Uniform variable location, or -1, if specified uniform variable name does not exist.
	*/
	GLint getUniformLocation(const std::string& varName);

	GLuint _shaderProgramID;  ///< OpenGL-assigned shader program ID
	bool _isLinked = false; ///< Flag teling, whether shader program has been linked successfully
	std::map<std::string, GLint> _uniformLocations; ///< Cache of uniform locations (reduces OpenGL calls)
};
