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

bool Texture::createFromData(const unsigned char* data, GLsizei width, GLsizei height, GLenum format, bool generateMipmaps)
{
    if (isLoaded()) {
        return false;
    }

    width_ = width;
    height_ = height;
    format_ = format;

    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glTexImage2D(GL_TEXTURE_2D, 0, format_, width_, height_, 0, format_, GL_UNSIGNED_BYTE, data);

    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}

bool Texture::loadTexture2D(const std::string& filePath, bool generateMipmaps)
{
    stbi_set_flip_vertically_on_load(1);
    int bytesPerPixel;
    const auto imageData = stbi_load(filePath.c_str(), &width_, &height_, &bytesPerPixel, 0);
    if (imageData == nullptr)
    {
        std::cout << "Failed to load image " << filePath << "!" << std::endl;
        return false;
    }

    GLenum format = 0;
    if (bytesPerPixel == 4) {
        format = GL_RGBA;
    }
    else if (bytesPerPixel == 3) {
        format = GL_RGB;
    }
    else if (bytesPerPixel == 1) {
        format = GL_DEPTH_COMPONENT;
    }

    const auto result = createFromData(imageData, width_, height_, format, generateMipmaps);
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
    if (!isLoadedCheck()) {
        return;
    }

    glDeleteTextures(1, &textureID_);
    textureID_ = 0;
    width_ = height_ = 0;
    format_ = 0;
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

bool Texture::isLoaded() const
{
    return textureID_ != 0;
}

bool Texture::resize(GLsizei newWidth, GLsizei newHeight)
{
    if (!isLoadedCheck()) {
        return false;
    }

    const auto oldFormat = format_;
    deleteTexture();
   
    return createFromData(nullptr, newWidth, newHeight, oldFormat, false);
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
    if (!isLoaded())
    {
        std::cout << "Attempting to access non loaded texture!" << std::endl;
        return false;
    }

    return true;
}
