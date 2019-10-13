#pragma once

#include <string>

#include <glad/glad.h>

/**
  Wraps OpenGL texture into convenient class.
*/
class Texture
{
public:
	~Texture();

	bool createFromData(const unsigned char* data, int width, int height, int bytesPerPixel, bool generateMipmaps = false);

	/** \brief  Loads image file as 2D OpenGL texture.
	*   \param  fileName        path to an image file
	*   \param  generateMipmaps true, if mipmaps should be generated automatically
	*   \return True, if the texture have been loaded correctly or false otherwise.
	*/
	bool loadTexture2D(const std::string& filePath, bool generateMipmaps = true);

	/** \brief  Binds texture to specified texture unit.
	*   \param  textureUnit  Texture unit index (default is 0)
	*/
	void bind(int textureUnit = 0) const;

	/** \brief  Deletes texture, but only, if it has been loaded correctly. */
	void deleteTexture();

	/** \brief  Gets file path of the texture if the texture has been loaded from file.
	*   \return File path of the texture or empty string otherwise.
	*/
	std::string getFilePath() const;

	/** \brief  Gets width of the texture (in pixels).
	*   \return Width of texture, in pixels.
	*/
	int getWidth() const;

	/** \brief  Gets height of the texture (in pixels).
	*   \return Height of texture, in pixels.
	*/
	int getHeight() const;

	/** \brief  Gets amount of bytes each pixel is represented with.
	*   \return Bytes per pixel of texture.
	*/
	int getBytesPerPixel() const;

	/** \brief  Gets number of texture units on current hardware.
	*   \return Number of texture units.
	*/
	static int getNumTextureImageUnits();
	
private:
	GLuint _textureID = 0; //!< OpenGL-assigned texture ID
	int _width = 0; //!< Width of texture in pixels
	int _height = 0; //!< Height of texture in pixels
	int _bytesPerPixel = 0; //!< Amount of bytes every pixel is represented with
	bool _isLoaded = false; //!< Flag holding, if texture has been loaded correctly
	std::string _filePath; //!< Path of file from which the texture has been loaded (might be empty, if texture was simply created from data)

	/** \brief  Checks, if the texture has been loaded correctly and if not, logs it into console.
	*   \return True, if texture has been loaded correctly or false otherwise.
	*/
	bool isLoadedCheck() const;
};