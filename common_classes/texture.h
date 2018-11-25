#pragma once

#include <string>

#include <glad/glad.h>

class Texture
{
public:
	bool loadTexture2D(const std::string& fileName, bool generateMipmaps = true);
	void bind(int textureUnit = 0) const;
	void deleteTexture();

	int getWidth() const;
	int getHeight() const;
	int getBytesPerPixel() const;
	
private:
	GLuint _textureID = 0;
	int _width = 0;
	int _height = 0;
	int _bytesPerPixel = 0;
	bool _isLoaded = false;

	bool isLoadedCheck() const;
};