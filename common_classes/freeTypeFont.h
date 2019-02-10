#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "vertexBufferObject.h"
#include "texture.h"
#include "sampler.h"
#include "shaderProgram.h"
#include "stringUtils.h"

/**
  Class maintaining TrueType font and its rendering using OpenGL.
*/
class FreeTypeFont
{
public:
	static const int CHARACTERS_TEXTURE_SIZE; //!< Size of texture atlas (in pixels) that stores characters
	static const std::string FREETYPE_FONT_PROGRAM_KEY; //!< Key for shader program for rendering fonts in 2D
	static const std::string FREETYPE_FONT_SAMPLER_KEY; //!< Key for sampler used for rendering fonts

	FreeTypeFont();
	~FreeTypeFont();

	/** \brief  Adds character range to load fonts from.
	*   \param  characterFrom First Unicode character
	*   \param  characterTo Last Unicode character
	*/
	void addCharacterRange(unsigned int characterFrom, unsigned int characterTo);

	/** \brief  Loads TrueType font from given file.
	*   \param  fontFilePath File path of the font
	*   \param  pixelSize Pixel size to load characters with
	*   \return True, if the font has been loaded or false otherwise.
	*/
	bool loadFont(const std::string& fontFilePath, int pixelSize);

	/** \brief  Gets width of a given text and given pixel size.
	*   \param  text Text to get width of
	*   \param  pixelSize Used pixel size. If -1, the loaded pixel size is used instead
	*   \return Text width rounded to next integer (ceiling).
	*/
	int getTextWidth(const std::string& text, int pixelSize = -1) const;

	/** \brief  Gets height of text with given pixel size.
	*   \param  pixelSize Used pixel size
	*   \return Text height rounded to next integer (ceiling).
	*/
	int getTextHeight(int pixelSize = -1) const;

	//* \brief  Prints text at given position with default (loaded) pixel size. */
	template <typename... Args>
	void print(int x, int y, const std::string& text, const Args&... args) const
	{
		printInternal(x, y, string_utils::formatString(text.c_str(), args...), -1);
	}

	//* \brief  Prints text at given position with given pixel size. */
	template <typename... Args>
	void printWithCustomSize(int x, int y, int pixelSize, const std::string& text, const Args&... args) const
	{
		print(x, y, text, pixelSize, string_utils::formatString(text.c_str(), args...));
	}

	//* \brief Deletes font with all of its data. */
	void deleteFont();

private:
	/** \brief  Prints text at given position with given pixel size.
	*   \param  x X position of text
	*   \param  x Y position of text
	*   \param  x text Text to print
	*   \param  pixelSize Used pixel size. If -1, the loaded pixel size is used instead
	*/
	void printInternal(int x, int y, const std::string& text, int pixelSize) const;

	/** \brief  Gets shader program for FreeType fonts.
	*   \return Shader program for FreeType fonts.
	*/
	ShaderProgram& getFreetypeFontShaderProgram() const;

	/** \brief  Gets sampler for FreeType fonts.
	*   \return FreeType fonts sampler.
	*/
	const Sampler& getFreetypeFontSampler() const;

	/**
	  Holds properties of one character.
	*/
	struct CharacterProperties
	{
		int characterCode; //!< What character code these properties belong to

		// Following properties come from FreeType directly
		int width;
		int height;
		int advanceX;
		int bearingX;
		int bearingY;

		// These are our properties used for rendering
		int renderIndex;
		int textureIndex;
	};

	/**
	  Holds unicode character range.
	*/
	struct CharacterRange
	{
		CharacterRange(unsigned int characterCodeFrom, unsigned int characterCodeTo)
		{
			this->characterCodeFrom = characterCodeFrom;
			this->characterCodeTo = characterCodeTo;
		}

		unsigned int characterCodeFrom; //!< Start of unicode range
		unsigned int characterCodeTo; //!< End of unicode range
	};
	
	bool _isLoaded = false; //!< Flag saying, if the font is loaded already
	std::vector<CharacterRange> _characterRanges; //!< List of unicode ranges to load characters from
	int _pixelSize; //!< Loaded pixel size of characters

	std::vector<std::unique_ptr<Texture>> _textures; //!< Vector holding all texture atlases
	std::map<int, CharacterProperties> _characterProperties; //!< Map holding properties of each loaded character
	GLuint _vao; //!< VAO for characters rendering
	VertexBufferObject _vbo; //!< VBO holding characters rendering data
};
