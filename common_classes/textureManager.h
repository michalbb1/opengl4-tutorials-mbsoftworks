#pragma once

#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>

#include "texture.h"

class TextureManager
{
public:
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;

	/** \brief  Gets the one and only instance of the texture manager.
	*   \return Texture manager instance.
	*/
	static TextureManager& getInstance();

	/** \brief  Loads image file as 2D OpenGL texture and stores it with specified key.
	*   \param  key             key to store texture with
	*   \param  fileName        path to an image file
	*   \param  generateMipmaps true, if mipmaps should be generated automatically
	*/
	void loadTexture2D(const std::string& key, const std::string& fileName, bool generateMipmaps = true);

	/** \brief  Retrieves texture with a specified key.
	*   \param  key Key to restore texture from
	*   \return Texture instance from a specified key.
	*/
	const Texture& getTexture(const std::string& key) const;

	/** \brief  Checks, if texture with specified key exists.
	*   \param  key Texture key to check existence of
	*   \return True if texture exists or false otherwise.
	*/
	bool containsTexture(const std::string& key) const;

	/** \brief  Deletes a texture from the manager. If it does not exist, does not do anything.
	*   \param  key Texture key to delete
	*   \return True if texture exists and has been deleted or false otherwise.
	*/
	bool deleteTexture(const std::string& key);

	/** \brief  Deletes all the textures loaded and clears the texture cache. */
	void clearTextureCache();

private:
	TextureManager() {}

	std::map<std::string, std::unique_ptr<Texture>> _textureCache; //!< Texture cache - stores textures within their keys in std::map
};