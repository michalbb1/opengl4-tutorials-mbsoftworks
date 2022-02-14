#pragma once

// STL
#include <vector>
#include <string>
#include <set>

// GLAD
#include <glad/glad.h>

#define DEFINE_SHADER_CONSTANT(constantName, constantValue) \
static const std::string constantName()                     \
{                                                           \
    static std::string value = constantValue;               \
    return value;                                           \
}

#define DEFINE_SHADER_CONSTANT_INDEX(constantName, constantValue)          \
DEFINE_SHADER_CONSTANT(constantName, constantValue)                        \
static const std::string constantName(const int index)                     \
{                                                                          \
    return std::string(constantValue) + "[" + std::to_string(index) + "]"; \
}

/**
 * Storage for commonly used shaders throughout the tutorials.
 */
class ShaderKeys
{
public:
    DEFINE_SHADER_CONSTANT(ambientLight, "ambientLight");
    DEFINE_SHADER_CONSTANT(diffuseLight, "diffuseLight");
    DEFINE_SHADER_CONSTANT(specularHighlight, "specularHighlight");
    DEFINE_SHADER_CONSTANT(pointLight, "pointLight");
    DEFINE_SHADER_CONSTANT(fog, "fog");
};

/**
 * Storage for commonly used constants in shader programs.
 */
class ShaderConstants
{
public:
    // Matrices
    DEFINE_SHADER_CONSTANT(modelMatrix,      "matrices.modelMatrix");
    DEFINE_SHADER_CONSTANT(projectionMatrix, "matrices.projectionMatrix");
    DEFINE_SHADER_CONSTANT(viewMatrix,       "matrices.viewMatrix");
    DEFINE_SHADER_CONSTANT(normalMatrix,     "matrices.normalMatrix");

    // Color and textures
    DEFINE_SHADER_CONSTANT(color, "color");
    DEFINE_SHADER_CONSTANT(sampler, "sampler");

    // Lighting
    DEFINE_SHADER_CONSTANT(ambientLight, "ambientLight");
    DEFINE_SHADER_CONSTANT(diffuseLight, "diffuseLight");
    DEFINE_SHADER_CONSTANT(pointLightA, "pointLightA");
    DEFINE_SHADER_CONSTANT(pointLightB, "pointLightB");
    DEFINE_SHADER_CONSTANT(normalLength, "normalLength");
    DEFINE_SHADER_CONSTANT(material, "material");
    DEFINE_SHADER_CONSTANT(eyePosition, "eyePosition");
    DEFINE_SHADER_CONSTANT(numPointLights, "numPointLights");
    
    // Fog constants
    DEFINE_SHADER_CONSTANT(fogParams, "fogParams");

    // MD2 Animation
    DEFINE_SHADER_CONSTANT(interpolationFactor, "interpolationFactor")
};

/**
 * Wraps OpenGL shader loading and compilation into a very convenient class.
 */
class Shader
{
public:
    ~Shader();

    /**
     * Loads and compiles shader from a specified file.
     *
     * @param fileName    path to a file
     * @param shaderType  type of shader (vertex, fragment, geometry...)
     *
     * @return True, if the shader has been successfully loaded and compiled, false otherwise.
     */
    bool loadShaderFromFile(const std::string& fileName, GLenum shaderType);

    /**
     * Checks, if shader is loaded and compiled successfully.
     *
     * @return True, if the shader has been successfully loaded and compiled, false otherwise.
     */
    bool isCompiled() const;

    /**
     * Deletes shader object from OpenGL.
     */
    void deleteShader();

    /**
     * Gets OpenGL assigned shader ID.
     */
    GLuint getShaderID() const;

    /**
     * Gets shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...).
     */
    GLenum getShaderType() const;

private:
    /**
     * Gets all lines from specified shader file.
     *
     * @param fileName               Filename to read the line from.
     * @param result                 std::vector to store the source code lines into
     * @param isReadingIncludedFile  Flag saying, whether we are reading file that's been included, that is only #include_part.
     *
     * @return True, if the loading has been successful, or false otherwise.
     */
    bool getLinesFromFile(const std::string& fileName, std::vector<std::string>& result, std::set<std::string>& filesIncludedAlready, bool isReadingIncludedFile = false) const;

    GLuint shaderID_{ 0 }; // OpenGL-assigned shader ID
    GLenum shaderType_{ 0 }; // Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...)
    bool isCompiled_{ false }; // Flag telling, whether shader has been loaded and compiled successfully
};
