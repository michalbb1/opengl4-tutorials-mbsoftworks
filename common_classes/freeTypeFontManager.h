#pragma once

// STL
#include <string>
#include <map>
#include <memory>

// Project
#include "freeTypeFont.h"

/**
 * Singleton class that manages and keeps track of all FreeType fonts used in the program.
 */
class FreeTypeFontManager
{
public:
    /**
     * Gets the one and only instance of the FreeType font manager.
     */
    static FreeTypeFontManager& getInstance();

    /**
     * Loads FreeType font from the given file with given pixel size and stores it with specified key.
     *
     * @param key        Key to store FreeType font with
     * @param filePath   Path to FreeType font file
     * @param pixelSize  Pixel size to load FreeType font with
     */
    void loadFreeTypeFontFromFile(const std::string& key, const std::string& filePath, const int pixelSize);

    /**
     * Loads FreeType font from the system by its name with given pixel size and stores it with specified key.
     *
     * @param key        Key to store FreeType font with
     * @param fontName   Name of the font to load
     * @param pixelSize  Pixel size to load FreeType font with
     */
    void loadSystemFreeTypeFont(const std::string& key, const std::string& fontName, const int pixelSize);

    /**
     * Retrieves FreeType font with a specified key.
     *
     * @param key  Key to restore FreeType font from
     *
     * @return FreeType font instance from a specified key.
     */
    FreeTypeFont& getFreeTypeFont(const std::string& key) const;

    /**
     * Deletes all the loaded FreeType fonts and clears the FreeType fonts cache.
     */
    void clearFreeTypeFontCache();

private:
    FreeTypeFontManager() {} // Private constructor to make class singleton
    FreeTypeFontManager(const FreeTypeFontManager&) = delete; // No copy constructor allowed
    void operator=(const FreeTypeFontManager&) = delete; // No copy assignment allowed

    /**
     * Checks, if FreeType font with specified key exists.
     *
     * @param key  FreeType font key to check existence of
     *
     * @return True if FreeType font exists, or false otherwise.
     */
    bool containsFreeTypeFont(const std::string& key) const;

    /**
     * Gets the directory path, where the system fonts are stored.
     */
    static const std::string& getSystemFontDirectory();
    
    std::map<std::string, std::unique_ptr<FreeTypeFont>> _freeTypeFontCache; // FreeType font cache - stores FreeType fonts within their keys in std::map
};