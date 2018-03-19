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
	if (!_isLinked)
		glUseProgram(_shaderProgramID);
}

void ShaderProgram::deleteProgram()
{
	if (!_isLinked)
		return;

	glDeleteProgram(_shaderProgramID);
	_isLinked = false;
}

GLuint ShaderProgram::getShaderProgramID()
{
	return _shaderProgramID;
}

