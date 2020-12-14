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
    if (_isLoaded) {
        return false;
    }

    _width = width;
    _height = height;
    _bytesPerPixel = bytesPerPixel;

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    GLenum internalFormat = 0;
    GLenum format = 0;
    if (_bytesPerPixel == 4) {
        internalFormat = format = GL_RGBA;
    }
    else if (_bytesPerPixel == 3) {
        internalFormat = format = GL_RGB;
    }
    else if (_bytesPerPixel == 1) {
        internalFormat = format = GL_DEPTH_COMPONENT;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    _isLoaded = true;
    return true;
}

bool Texture::loadTexture2D(const std::string& filePath, bool generateMipmaps)
{
    stbi_set_flip_vertically_on_load(1);
    const auto imageData = stbi_load(filePath.c_str(), &_width, &_height, &_bytesPerPixel, 0);
    if (imageData == nullptr)
    {
        std::cout << "Failed to load image " << filePath << "!" << std::endl;
        return false;
    }

    auto result = createFromData(imageData, _width, _height, _bytesPerPixel, generateMipmaps);
    stbi_image_free(imageData);
    _filePath = filePath;
    return result;
}

void Texture::bind(const int textureUnit) const
{
    if (!isLoadedCheck()) {
        return;
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::deleteTexture()
{
    if (!isLoadedCheck()) {
        return;
    }

    glDeleteTextures(1, &_textureID);
    _isLoaded = false;
}

std::string Texture::getFilePath() const
{
    return _filePath;
}

int Texture::getWidth() const
{
    return _width;
}

int Texture::getHeight() const
{
    return _height;
}

int Texture::getBytesPerPixel() const
{
    return _bytesPerPixel;
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
    if (!_isLoaded)
    {
        std::cout << "Attempting to access non loaded texture!" << std::endl;
        return false;
    }

    return true;
}
