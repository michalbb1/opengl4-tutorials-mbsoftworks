// STL
#include <iostream>

// Project
#include "shaderProgram.h"

ShaderProgram::~ShaderProgram()
{
    deleteProgram();
}

void ShaderProgram::createProgram()
{
    shaderProgramID_ = glCreateProgram();
}

bool ShaderProgram::addShaderToProgram(const Shader& shader) const
{
    if (!shader.isCompiled())
        return false;

    glAttachShader(shaderProgramID_, shader.getShaderID());
    return true;
}

bool ShaderProgram::linkProgram()
{
	if (_isLinked) {
		return true;
	}

    // Try to link program and get the link status
    glLinkProgram(shaderProgramID_);
    GLint linkStatus;
    glGetProgramiv(shaderProgramID_, GL_LINK_STATUS, &linkStatus);
    _isLinked = linkStatus == GL_TRUE;

    if (!_isLinked)
    {
        std::cerr << "Error! Shader program wasn't linked!";

        // Get length of the error log first
        GLint logLength;
        glGetProgramiv(shaderProgramID_, GL_INFO_LOG_LENGTH, &logLength);

        // If there is some log, then retrieve it and output extra information
        if (logLength > 0)
        {
            GLchar* logMessage = new GLchar[logLength];
            glGetProgramInfoLog(shaderProgramID_, logLength, nullptr, logMessage);
            std::cerr << " The linker returned: " << std::endl << std::endl << logMessage;
            delete[] logMessage;
        }

        std::cerr << std::endl;
        return false;
    }

    return _isLinked;
}

void ShaderProgram::useProgram() const
{
    if (_isLinked) {
        glUseProgram(shaderProgramID_);
    }
}

void ShaderProgram::deleteProgram()
{
    if (shaderProgramID_ == 0) {
        return;
    }

    std::cout << "Deleting shader program with ID " << shaderProgramID_ << std::endl;
    glDeleteProgram(shaderProgramID_);
    _isLinked = false;
}

GLuint ShaderProgram::getShaderProgramID() const
{
    return shaderProgramID_;
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

    const auto result = glGetUniformBlockIndex(shaderProgramID_, uniformBlockName.c_str());
    if (result == GL_INVALID_INDEX) {
        std::cerr << "Could not get index of uniform block " << uniformBlockName << ", check if such uniform block really exists!" << std::endl;
    }

    return result;
}

void ShaderProgram::bindUniformBlockToBindingPoint(const std::string& uniformBlockName, const GLuint bindingPoint) const
{
    const auto blockIndex = getUniformBlockIndex(uniformBlockName);
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(shaderProgramID_, blockIndex, bindingPoint);
    }
}

void ShaderProgram::setTransformFeedbackRecordedVariables(const std::vector<std::string>& recordedVariablesNames, const GLenum bufferMode) const
{
	std::vector<const char*> recordedVariablesNamesPtrs;
	for (const auto& recordedVariableName : recordedVariablesNames) {
		recordedVariablesNamesPtrs.push_back(recordedVariableName.c_str());
	}

	glTransformFeedbackVaryings(shaderProgramID_, static_cast<GLsizei>(recordedVariablesNamesPtrs.size()), recordedVariablesNamesPtrs.data(), bufferMode);
}
