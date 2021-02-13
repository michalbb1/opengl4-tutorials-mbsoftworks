// STL
#include <stdexcept>

// Project
#include "samplerManager.h"

SamplerManager& SamplerManager::getInstance()
{
    static SamplerManager sm;
    return sm;
}

Sampler& SamplerManager::createSampler(const std::string& key, const MagnificationFilter magnificationFilter, const MinificationFilter minificationFilter)
{
    if (containsSampler(key))
    {
        const auto msg = "Sampler with key '" + key + "' already exists!";
        throw std::runtime_error(msg.c_str());
    }

    std::unique_ptr<Sampler> sampler = std::make_unique<Sampler>();
    sampler->create();
    sampler->bind();
    sampler->setMagnificationFilter(magnificationFilter);
    sampler->setMinificationFilter(minificationFilter);
    _samplerCache[key] = std::move(sampler);

    return getSampler(key);
}

Sampler& SamplerManager::getSampler(const std::string& key) const
{
    if (!containsSampler(key))
    {
        const auto msg = "Attempting to get non-existing sampler with key '" + key + "'!";
        throw std::runtime_error(msg.c_str());
    }

    return *_samplerCache.at(key);
}

void SamplerManager::clearSamplerCache()
{
    _samplerCache.clear();
}

bool SamplerManager::containsSampler(const std::string & key) const
{
    return _samplerCache.count(key) > 0;
}
