// STL
#include <iostream>
#include <mutex>

// STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Project
#include "texture.h"

Texture::~Texture()
{
    deleteTexture();
}

bool Texture::createFromData(const unsigned char* data, int width, int height, int bytesPerPixel, bool generateMipmaps)
{
    if (isLoaded_) {
        return false;
    }

    width_ = width;
    height_ = height;
    bytesPerPixel_ = bytesPerPixel;

    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    GLenum internalFormat = 0;
    GLenum format = 0;
    if (bytesPerPixel_ == 4) {
        internalFormat = format = GL_RGBA;
    }
    else if (bytesPerPixel_ == 3) {
        internalFormat = format = GL_RGB;
    }
    else if (bytesPerPixel_ == 1) {
        internalFormat = format = GL_DEPTH_COMPONENT;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);

    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    isLoaded_ = true;
    return true;
}

bool Texture::loadTexture2D(const std::string& filePath, bool generateMipmaps)
{
    stbi_set_flip_vertically_on_load(1);
    const auto imageData = stbi_load(filePath.c_str(), &width_, &height_, &bytesPerPixel_, 0);
    if (imageData == nullptr)
    {
        std::cout << "Failed to load image " << filePath << "!" << std::endl;
        return false;
    }

    const auto result = createFromData(imageData, width_, height_, bytesPerPixel_, generateMipmaps);
    stbi_image_free(imageData);
    filePath_ = filePath;
    return result;
}

void Texture::bind(const int textureUnit) const
{
    if (!isLoadedCheck()) {
        return;
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID_);
}

void Texture::deleteTexture()
{
    if (!isLoaded_) {
        return;
    }

    glDeleteTextures(1, &textureID_);
    textureID_ = 0;
    isLoaded_ = false;
}

GLuint Texture::getID() const
{
    return textureID_;
}

std::string Texture::getFilePath() const
{
    return filePath_;
}

int Texture::getWidth() const
{
    return width_;
}

int Texture::getHeight() const
{
    return height_;
}

int Texture::getBytesPerPixel() const
{
    return bytesPerPixel_;
}

bool Texture::resize(GLsizei newWidth, GLsizei newHeight)
{
    if (!isLoaded_) {
        return false;
    }

    deleteTexture();
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    GLenum internalFormat = 0;
    GLenum format = 0;
    if (bytesPerPixel_ == 4) {
        internalFormat = format = GL_RGBA;
    }
    else if (bytesPerPixel_ == 3) {
        internalFormat = format = GL_RGB;
    }
    else if (bytesPerPixel_ == 1) {
        internalFormat = format = GL_DEPTH_COMPONENT;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, newWidth, newHeight, 0, format, GL_UNSIGNED_BYTE, nullptr);

    if (true) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    width_ = newWidth;
    height_ = newHeight;
    isLoaded_ = true;

    return true;
}

int Texture::getNumTextureImageUnits()
{
    static std::once_flag queryOnceFlag;
    static int maxTextureUnits;
    std::call_once(queryOnceFlag, []() {glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits); });
    
    return maxTextureUnits;
}

bool Texture::isLoadedCheck() const
{
    if (!isLoaded_)
    {
        std::cout << "Attempting to access non loaded texture!" << std::endl;
        return false;
    }

    return true;
}
