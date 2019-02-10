#pragma once

#include <string>
#include <map>
#include <memory>

#include "freetypeFont.h"

class FreeTypeFontManager
{
public:
	FreeTypeFontManager(const FreeTypeFontManager&) = delete;
	void operator=(const FreeTypeFontManager&) = delete;

	/** \brief  Gets the one and only instance of the FreeType font manager.
	*   \return FreeType font manager instance.
	*/
	static FreeTypeFontManager& getInstance();

	/** \brief  Loads FreeType font from the given file with given pixel size and stores it with specified key.
	*   \param  key Key to store FreeType font with
	*   \param  filePath Path to FreeType font file
	*   \param  pixelSize Pixel size to load FreeType font with
	*/
	void loadFreeTypeFontFromFile(const std::string& key, const std::string& filePath, const int pixelSize);

	/** \brief  Loads FreeType font from the system by its name with given pixel size and stores it with specified key.
	*   \param  key Key to store FreeType font with
	*   \param  fontName Name of the font to load
	*   \param  pixelSize Pixel size to load FreeType font with
	*/
	void loadSystemFreeTypeFont(const std::string& key, const std::string& fontName, const int pixelSize);

	/** \brief  Retrieves FreeType font with a specified key.
	*   \param  key Key to restore FreeType font from
	*   \return FreeType font instance from a specified key.
	*/
	FreeTypeFont& getFreeTypeFont(const std::string& key) const;

	/** \brief  Deletes all the loaded FreeType fonts and clears the FreeType fonts cache. */
	void clearFreeTypeFontCache();

private:
	FreeTypeFontManager() {}

	/** \brief  Checks, if FreeType font with specified key exists.
	*   \param  key FreeType font key to check existence of
	*   \return True if FreeType font exists, or false otherwise.
	*/
	bool containsFreeTypeFont(const std::string& key) const;

	/** \brief  Gets the directory path, where the system fonts are stored.
	*   \return System directory, where fonts are stored.
	*/
	static const std::string& getSystemFontDirectory();
	
	std::map<std::string, std::unique_ptr<FreeTypeFont>> _freeTypeFontCache; //!< FreeType font cache - stores FreeType fonts within their keys in std::map
};