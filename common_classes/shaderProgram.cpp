#include "shaderProgram.h"

#include <iostream>

void ShaderProgram::createProgram()
{
	_shaderProgramID = glCreateProgram();
}

bool ShaderProgram::addShaderToProgram(const Shader& shader)
{
	if (!shader.isLoaded())
		return false;

	glAttachShader(_shaderProgramID, shader.getShaderID());

	return true;
}

bool ShaderProgram::linkProgram()
{
	glLinkProgram(_shaderProgramID);
	int linkStatus;
	glGetProgramiv(_shaderProgramID, GL_LINK_STATUS, &linkStatus);
	_isLinked = linkStatus == GL_TRUE;

	if (!_isLinked)
	{
		char infoLogBuffer[2048];
		int logLength;
		glGetProgramInfoLog(_shaderProgramID, 2048, &logLength, infoLogBuffer);

		std::cout << "Error! Shader program wasn't linked! The linker returned: " << std::endl << std::endl << infoLogBuffer << std::endl;

		return false;
	}
	return _isLinked;
}

void ShaderProgram::useProgram()
{
	if (_isLinked)
		glUseProgram(_shaderProgramID);
}

void ShaderProgram::deleteProgram()
{
	if (!_isLinked)
		return;

	glDeleteProgram(_shaderProgramID);
	_isLinked = false;
}

GLuint ShaderProgram::getShaderProgramID() const
{
	return _shaderProgramID;
}

Uniform& ShaderProgram::operator[](const std::string& varName)
{
	if (_uniforms.count(varName) == 0)
	{
		_uniforms[varName] = Uniform(varName, this);
	}

	return _uniforms[varName];
}

// Model and normal matrix setting is pretty common, that's why this convenience function

void ShaderProgram::setModelAndNormalMatrix(const std::string& modelMatrixName, const std::string& normalMatrixName, const glm::mat4& modelMatrix)
{
	//setUniform(modelMatrixName, modelMatrix);
//	setUniform(normalMatrixName, glm::transpose(glm::inverse(glm::mat3(modelMatrix))));
}