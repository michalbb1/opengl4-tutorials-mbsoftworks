#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

/**
  Wraps OpenGL shader loading and compilation into a very convenient class.
*/

class Shader
{
public:

	/** \brief Loads and compiles shader from a specified file.
	*  \param  fileName   path to a file
	*  \param  shaderType type of shader (fragment, vertex, geometry...)
	*  \return True, if the shader has been successfully loaded and compiled, false otherwise.
	*/
	bool loadShaderFromFile(const std::string& fileName, GLenum shaderType);

	bool isLoaded();
	void deleteShader();

	GLuint getShaderID();
	GLenum getShaderType();

private:

	bool getLinesFromFile(const std::string& fileName, std::vector<std::string>& vResult, bool isReadingIncludedFile = false);

	GLuint _shaderID; // ID of shader
	GLenum _shaderType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool _isLoaded = false; // Whether shader was loaded and compiled
};
