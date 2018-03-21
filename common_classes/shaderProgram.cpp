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

void ShaderProgram::setUniform(const std::string& varName, const glm::vec2& vector2D)
{
	glUniform2fv(getUniformLocation(varName), 1, (GLfloat*)&vector2D);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::vec2* vectors2D, int count)
{
	glUniform2fv(getUniformLocation(varName), count, (GLfloat*)vectors2D);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::vec3& vector3D)
{
	glUniform3fv(getUniformLocation(varName), 1, (GLfloat*)&vector3D);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::vec3* vectors3D, int count)
{
	glUniform3fv(getUniformLocation(varName), count, (GLfloat*)vectors3D);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::vec4& vector4D)
{
	glUniform4fv(getUniformLocation(varName), 1, (GLfloat*)&vector4D);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::vec4* vectors4D, int count)
{
	glUniform4fv(getUniformLocation(varName), count, (GLfloat*)vectors4D);
}

void ShaderProgram::setUniform(const std::string& varName, const float floatValue)
{
	glUniform1fv(getUniformLocation(varName), 1, (GLfloat*)&floatValue);
}

void ShaderProgram::setUniform(const std::string& varName, const float* floatValues, int count)
{
	glUniform1fv(getUniformLocation(varName), count, (GLfloat*)floatValues);
}

void ShaderProgram::setUniform(const std::string& varName, const int integerValue)
{
	glUniform1iv(getUniformLocation(varName), 1, (GLint*)integerValue);
}

void ShaderProgram::setUniform(const std::string& varName, const int* integerValues, int count)
{
	glUniform1iv(getUniformLocation(varName), count, (GLint*)integerValues);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::mat3& matrix)
{
	glUniformMatrix3fv(getUniformLocation(varName), 1, false, (GLfloat*)&matrix);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::mat3* matrices, int count)
{
	glUniformMatrix3fv(getUniformLocation(varName), count, false, (GLfloat*)matrices);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::mat4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(varName), 1, false, (GLfloat*)&matrix);
}

void ShaderProgram::setUniform(const std::string& varName, const glm::mat4* matrices, int count)
{
	glUniformMatrix4fv(getUniformLocation(varName), 1, false, (GLfloat*)matrices);
}

void ShaderProgram::setModelAndNormalMatrix(const std::string& modelMatrixName, const std::string& normalMatrixName, const glm::mat4& modelMatrix)
{
	setUniform(modelMatrixName, modelMatrix);
	setUniform(normalMatrixName, glm::transpose(glm::inverse(glm::mat3(modelMatrix))));
}

GLint ShaderProgram::getUniformLocation(const std::string& varName)
{
	if (_uniformLocations.count(varName) > 0)
	{
		_uniformLocations[varName] = glGetUniformLocation(_shaderProgramID, varName.c_str());
	}

	return _uniformLocations[varName];
}

