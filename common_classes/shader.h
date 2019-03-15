#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

#define DEFINE_SHADER_CONSTANT(constantName, constantValue) \
static const std::string constantName()                     \
{                                                           \
	static std::string value = constantValue;               \
	return value;                                           \
}

/**
  Storage of all commonly used constants in shader programs.
*/
class ShaderConstants
{
public:
	DEFINE_SHADER_CONSTANT(modelMatrix,      "matrices.modelMatrix");
	DEFINE_SHADER_CONSTANT(projectionMatrix, "matrices.projectionMatrix");
	DEFINE_SHADER_CONSTANT(viewMatrix,       "matrices.viewMatrix");

	DEFINE_SHADER_CONSTANT(color, "color");
	DEFINE_SHADER_CONSTANT(sampler, "sampler");

	DEFINE_SHADER_CONSTANT(ambientLight, "ambientLight");
	DEFINE_SHADER_CONSTANT(diffuseLight, "diffuseLight");
};

/**
  Wraps OpenGL shader loading and compilation into a very convenient class.
*/
class Shader
{
public:
	~Shader();

	/** \brief Loads and compiles shader from a specified file.
	*   \param  fileName   path to a file
	*   \param  shaderType type of shader (fragment, vertex, geometry...)
	*   \return True, if the shader has been successfully loaded and compiled, false otherwise.
	*/
	bool loadShaderFromFile(const std::string& fileName, GLenum shaderType);

	/** \brief Checks, if shader is loaded successfully.
	*   \return True, if the shader has been successfully loaded and compiled, false otherwise.
	*/
	bool isLoaded() const;

	//* \brief Deletes shader object.
	void deleteShader();

	/** \brief Gets OpenGL-assigned shader ID.
	*   \return Shader ID.
	*/
	GLuint getShaderID() const;

	/** \brief Gets shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...).
	*   \return Shader Type.
	*/
	GLenum getShaderType() const;

private:

	/** \brief Gets all lines from specified shader file.
	*  \param  fileName                Filename to read the line from.
	*  \param  result                  std::vector to store the lines to
	*  \param  isReadingIncludedFile   Flag saying, whether we are reading file that's been included, that is only #include_part.
	*  \return True, if the loading has been successful, or false otherwise.
	*/
	bool getLinesFromFile(const std::string& fileName, std::vector<std::string>& result, bool isReadingIncludedFile = false);

	GLuint _shaderID; //!< OpenGL-assigned shader ID
	GLenum _shaderType; //!< Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...)
	bool _isLoaded = false;  //!< Flag teling, whether shader has been loaded and compiled sucessfully
};
