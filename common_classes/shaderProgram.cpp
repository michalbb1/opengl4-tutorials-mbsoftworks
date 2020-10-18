#include "shaderProgram.h"

#include <iostream>

ShaderProgram::~ShaderProgram()
{
	deleteProgram();
}

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

void ShaderProgram::useProgram() const
{
	if (_isLinked) {
		glUseProgram(_shaderProgramID);
	}
}

void ShaderProgram::deleteProgram()
{
	if (!_isLinked) {
		return;
	}

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

void ShaderProgram::setModelAndNormalMatrix(const glm::mat4& modelMatrix)
{
	(*this)[ShaderConstants::modelMatrix()] = modelMatrix;
	(*this)[ShaderConstants::normalMatrix()] = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}

GLuint ShaderProgram::getUniformBlockIndex(const std::string& uniformBlockName) const
{
    if (!_isLinked)
    {
        std::cerr << "Cannot get index of uniform block " << uniformBlockName << " when program has not been linked!" << std::endl;
        return GL_INVALID_INDEX;
    }

    GLuint result = glGetUniformBlockIndex(_shaderProgramID, uniformBlockName.c_str());
    if (result == GL_INVALID_INDEX) {
        std::cerr << "Could not get index of uniform block " << uniformBlockName << ", check if such uniform block really exists!" << std::endl;
    }

    return result;
}

void ShaderProgram::bindUniformBlockToBindingPoint(const std::string& uniformBlockName, const GLuint bindingPoint) const
{
    const auto blockIndex = getUniformBlockIndex(uniformBlockName);
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(_shaderProgramID, blockIndex, bindingPoint);
    }
}
