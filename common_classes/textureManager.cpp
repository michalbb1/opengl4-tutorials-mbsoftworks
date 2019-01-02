#include "textureManager.h"

TextureManager& TextureManager::getInstance()
{
	static TextureManager tm;
	return tm;
}

void TextureManager::loadTexture2D(const std::string& key, const std::string& fileName, bool generateMipmaps)
{
	if (containsTexture(key)) {
		return;
	}

	auto texturePtr = std::make_unique<Texture>();
	if (!texturePtr->loadTexture2D(fileName, generateMipmaps))
	{
		auto msg = "Could not load texture with key '" + key + "' from file '" + fileName + " '!";
		throw std::runtime_error(msg.c_str());
	}
	_textureCache[key] = std::move(texturePtr);
}

const Texture& TextureManager::getTexture(const std::string& key) const
{
	if (!containsTexture(key))
	{
		auto msg = "Attempting to get non-existing texture with key '" + key + "'!";
		throw std::runtime_error(msg.c_str());
	}

	return *_textureCache.at(key).get();
}

void TextureManager::clearTextureCache()
{
	_textureCache.clear();
}

bool TextureManager::containsTexture(const std::string& key) const
{
	return _textureCache.count(key) > 0;
}
