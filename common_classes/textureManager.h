#pragma once

// STL
#include <string>
#include <map>
#include <memory>

// Project
#include "texture.h"

/**
 * Singleton class that manages and keeps track of all textures in the program.
 */
class TextureManager
{
public:
    /**
     * Gets the one and only instance of the texture manager.
     */
    static TextureManager& getInstance();

    /**
     * Loads image file as 2D OpenGL texture and stores it with specified key.
     *
     * @param key              key to store texture with
     * @param fileName         path to an image file
     * @param generateMipmaps  true, if mipmaps should be generated automatically
    */
    void loadTexture2D(const std::string& key, const std::string& fileName, bool generateMipmaps = true);

    /**
     * Gets texture with a specified key.
     *
     * @param key  Key to get texture from
     *
     * @return Reference to Texture instance from a specified key.
     */
    const Texture& getTexture(const std::string& key) const;

    /**
     * Checks, if texture with specified key exists.
     *
     * @param key  Texture key to check existence of
     *
     * @return True if texture exists or false otherwise.
     */
    bool containsTexture(const std::string& key) const;

    /**
     * Checks, if texture with specified file path already exists.
     *
     * @param filePath  File path of the texture to check
     *
     * @return Texture key if texture with such path exists or empty string otherwise
     */
    std::string containsTextureWithPath(const std::string& filePath) const;

    /**
     * Deletes a texture from the manager. If it does not exist, does not do anything.
     *
     * @param key  Texture key to delete
     *   
     * @return True if texture exists and has been deleted or false otherwise.
     */
    bool deleteTexture(const std::string& key);

    /**
     * Deletes all the textures loaded and clears the texture cache.
     */
    void clearTextureCache();

private:
    TextureManager() {} // Private constructor to make class truly singleton
	TextureManager(const TextureManager&) = delete; // No copy constructor allowed
	void operator=(const TextureManager&) = delete; // No copy assignment allowed

    std::map<std::string, std::unique_ptr<Texture>> _textureCache; // Texture cache - stores textures within their keys in std::map
};