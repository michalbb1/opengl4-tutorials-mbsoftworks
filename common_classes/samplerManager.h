#pragma once

// STL
#include <string>
#include <map>
#include <memory>

// GLAD
#include <glad/glad.h>

// Project
#include "sampler.h"

/**
 * Singleton class that manages and keeps track of all samplers in the program.
 */
class SamplerManager
{
public:
    /**
     * Gets the one and only instance of the sampler manager.
     */
    static SamplerManager& getInstance();

    /**
     * Creates a new sampler and stores it with specified key.
     *
     * @param key  Key to store sampler with
     *
     * @return Sampler instance with specified key.
     */
    Sampler& createSampler(const std::string& key, const MagnificationFilter magnificationFilter, const MinificationFilter minificationFilter);

    /**
     * Retrieves sampler with a specified key.
     *
     * @param key  Key to restore sampler from
     *
     * @return Sampler instance from a specified key.
     */
    Sampler& getSampler(const std::string& key) const;

    /**
     * Deletes all samplers from OpenGL and clears the sampler cache.
     */
    void clearSamplerCache();

private:
    SamplerManager() {} // Private constructor to make class singleton
    SamplerManager(const SamplerManager&) = delete; // No copy constructor allowed
    void operator=(const SamplerManager&) = delete; // No copy assignment allowed

    /**
     * Checks, if sampler with specified key exists.
     *
     * @param key  Sampler key to check existence of
     *
     * @return True if sampler exists or false otherwise.
     */
    bool containsSampler(const std::string& key) const;

    std::map<std::string, std::unique_ptr<Sampler>> _samplerCache; // Sampler cache - stores samplers within their keys in std::map
};