#pragma once

// STL
#include <string>

// GLAD
#include <glad/glad.h>

/**
 *  Wraps OpenGL texture into convenient class.
 */
class Texture
{
public:
    ~Texture();

    /**
     * Creates texture from given raw data.
     *
     * @param data             Pointer to raw data of the texture
     * @param width            Width of the texture
     * @param height           Height of the texture
     * @param format           Format of the texture data (e.g. GL_RGB)
     * @param generateMipmaps  True, if mipmaps should be generated automatically
     *
     * @return True, if texture has been loaded correctly or false otherwise.
     */
    bool createFromData(const unsigned char* data, GLsizei width, GLsizei height, GLenum format, bool generateMipmaps = false);
    /**
     * Loads image file as 2D OpenGL texture.
     *   
     * @param filePath         Path to an image file
     * @param generateMipmaps  True, if mipmaps should be generated automatically
     *
     * @return True, if the texture has been loaded correctly or false otherwise.
     */
    bool loadTexture2D(const std::string& filePath, bool generateMipmaps = true);

    /**
     * Binds texture to specified texture unit.
     * 
     * @param textureUnit  Texture unit index (default is 0)
     */
    void bind(GLenum textureUnit = 0) const;

    /**
     * Deletes loaded texture from OpenGL. Does nothing if the texture has not been loaded correctly.
     */
    void deleteTexture();

    /**
     * Gets OpenGL-assigned texture ID
     */
    GLuint getID() const;

    /**
     * Gets file path of the texture if the texture has been loaded from file.
     *
     * @return File path of the texture or empty string otherwise.
     */
    std::string getFilePath() const;

    /**
     * Gets width of the texture (in pixels).
     */
    GLsizei getWidth() const;

    /** 
     * Gets height of the texture (in pixels).
     */
    GLsizei getHeight() const;

    bool isLoaded() const;

    bool resize(GLsizei newWidth, GLsizei newHeight);

    /**
     * Gets number of available OpenGL texture image units of current hardware.
     */
    static int getNumTextureImageUnits();
    
private:
    GLuint textureID_ = 0; // OpenGL-assigned texture ID
    GLsizei width_ = 0; // Width of texture in pixels
    GLsizei height_ = 0; // Height of texture in pixels
    GLenum format_ {0}; // Format this texture is represented with
    std::string filePath_; // Path of file from which the texture has been loaded (might be empty, if texture was simply created from data)

    /**
     * Checks, if the texture has been loaded correctly and if not, logs it into console.
     *
     * @return True, if texture has been loaded correctly or false otherwise.
     */
    bool isLoadedCheck() const;
};