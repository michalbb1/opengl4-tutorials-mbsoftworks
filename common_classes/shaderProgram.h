#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <string>

#include "shader.h"
#include "uniform.h"

/**
  Wraps OpenGL shader program creation and linking into a very convenient class.
*/

class ShaderProgram
{
public:
	~ShaderProgram();

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
	void useProgram() const;

	//* \brief Deletes this shader program.
	void deleteProgram();

	/** \brief Gets OpenGL-assigned shader program ID.
	*   \return Shader program ID.
	*/
	GLuint getShaderProgramID() const;

	/** \brief Gets uniform variable by name. If it's first time, it creates a new one, otherwise it's retrieved from cache.
	*   \param  varName uniform variable name
	*   \return Uniform variable, even if it does not exist (it will be invalid)
	*/
	Uniform& operator[](const std::string& varName);

	// Model and normal matrix setting is pretty common, that's why this convenience function
	void setModelAndNormalMatrix(const glm::mat4& modelMatrix);

private:

	GLuint _shaderProgramID;  //! OpenGL-assigned shader program ID
	bool _isLinked = false; //! Flag teling, whether shader program has been linked successfully
	std::map<std::string, Uniform> _uniforms; //! Cache of uniform locations (reduces OpenGL calls)
};
