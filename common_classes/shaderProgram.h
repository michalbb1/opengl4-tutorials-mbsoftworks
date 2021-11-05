#pragma once

// STL
#include <map>
#include <string>

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>

// Project
#include "shader.h"
#include "uniform.h"

/**
 * Wraps OpenGL shader program creation and linking into a very convenient class.
 */
class ShaderProgram
{
public:
    ~ShaderProgram();

    /**
     * Creates a new shader program.
     */
    void createProgram();

    /**
     * Adds a shader to shader program. Shader must be properly loaded and compiled.
     *
     * @return True, if the shader has been added or false otherwise.
     */
    bool addShaderToProgram(const Shader& shader) const;

    /**
     * Links the program. If the function succeeds, shader program is ready to use.
     *
     * @return True, if the shader has been linked or false otherwise.
     */
    bool linkProgram();

    /**
     * Uses this shader program (makes current).
     */
    void useProgram() const;

    /**
     * Deletes this shader program from OpenGL.
     */
    void deleteProgram();

    /**
     * Gets OpenGL-assigned shader program ID.
     */
    GLuint getShaderProgramID() const;

    /**
     * Gets uniform variable by name. If it's first time, it creates a new one, otherwise it's retrieved from cache.
     *
     * @param varName  uniform variable name
     *
     * @return Uniform variable, even if it does not exist (it will be invalid).
     */
    Uniform& operator[](const std::string& varName);

    /**
     * Sets model and normal matrix at once. Setting the two together is pretty common,
     * that's why this convenience function exists.
     *
     * @param modelMatrix  Model matrix to be set, normal matrix is calculated out of it
     */
    void setModelAndNormalMatrix(const glm::mat4& modelMatrix);

    /**
     * Gets index of given uniform block in this shader program.
     *
     * @param uniformBlockName  uniform block name
     *
     * @return Index of uniform block or GL_INVALID_INDEX if such block doesn't exist / some other error occurs.
     *
     * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetUniformBlockIndex.xhtml
     */
    GLuint getUniformBlockIndex(const std::string& uniformBlockName) const;

	/**
	 * Binds uniform block of this program to a uniform binding point.
	 *
	 * @param uniformBlockName  uniform block name
	 * @param bindingPoint      uniform binding point to bind the uniform block to
	 *
	 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniformBlockBinding.xhtml
	 */
	void bindUniformBlockToBindingPoint(const std::string& uniformBlockName, GLuint bindingPoint) const;

	/**
	 * Tells OpenGL, which output variables should be recorded during transform feedback.
     * From early OpenGL versions (3.0), a term varying was used for variables shared between shader stages,
     * but this name is deprecated for quite some time. But for legacy reasons, OpenGL function names
     * still use the term varying.
	 *
	 * @param recordedVariablesNames  names of the output variables to be recorded
	 * @param bufferMode              buffer mode to be used (usually GL_INTERLEAVED_ATTRIBS)
	 *
	 * @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTransformFeedbackVaryings.xhtml
	 */
	void setTransformFeedbackRecordedVariables(const std::vector<std::string>& recordedVariablesNames, GLenum bufferMode = GL_INTERLEAVED_ATTRIBS) const;

private:
    GLuint shaderProgramID_{ 0 }; // OpenGL-assigned shader program ID
    bool _isLinked{ false }; // Flag teling, whether shader program has been linked successfully
    std::map<std::string, Uniform> _uniforms; // Cache of uniform locations (reduces OpenGL calls)
};
