// STL
#include <mutex>

// Project
#include "freeTypeFontManager.h"

FreeTypeFontManager& FreeTypeFontManager::getInstance()
{
	static FreeTypeFontManager fm;
	return fm;
}

void FreeTypeFontManager::loadFreeTypeFontFromFile(const std::string& key, const std::string &filePath, const int pixelSize)
{
	if (containsFreeTypeFont(key))
	{
		const auto msg = "FreeType font with key '" + key + "' already exists!";
		throw std::runtime_error(msg.c_str());
	}

	auto freeTypeFont = std::make_unique<FreeTypeFont>();
	if (!freeTypeFont->loadFont(filePath, pixelSize))
	{
		const auto msg = "Could not load FreeType font'" + filePath + "'!";
		throw std::runtime_error(msg);
	}

	_freeTypeFontCache[key] = std::move(freeTypeFont);
}

void FreeTypeFontManager::loadSystemFreeTypeFont(const std::string& key, const std::string& fontName, const int pixelSize)
{
    #ifdef _WIN32
	loadFreeTypeFontFromFile(key, getSystemFontDirectory()+fontName, pixelSize);
    #else
	loadFreeTypeFontFromFile(key, "/usr/share/fonts/truetype/FreeMono.ttf");
    #endif
}

FreeTypeFont& FreeTypeFontManager::getFreeTypeFont(const std::string& key) const
{
	if (!containsFreeTypeFont(key))
	{
		const auto msg = "Attempting to get non-existing FreeType font with key '" + key + "'!";
		throw std::runtime_error(msg.c_str());
	}

	return *_freeTypeFontCache.at(key);
}

void FreeTypeFontManager::clearFreeTypeFontCache()
{
	_freeTypeFontCache.clear();
}

bool FreeTypeFontManager::containsFreeTypeFont(const std::string& key) const
{
	return _freeTypeFontCache.count(key) > 0;
}

const std::string& FreeTypeFontManager::getSystemFontDirectory()
{
	static std::string systemFontsDirectory;
	static std::once_flag prepareOnceFlag;

	std::call_once(prepareOnceFlag, []()
	{
		#ifdef _WIN32
		char buffer[512]; GetWindowsDirectory(buffer, 512);
		systemFontsDirectory = buffer;
		systemFontsDirectory += "\\Fonts\\";
		#else
		// TODO: check if this works
		systemFontsDirectory = "/usr/share/fonts/";
		#endif
	});

	return systemFontsDirectory;
}
