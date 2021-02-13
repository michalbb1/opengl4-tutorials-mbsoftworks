// STL
#include <stdexcept>

// Project
#include "shaderManager.h"

ShaderManager& ShaderManager::getInstance()
{
    static ShaderManager sm;
    return sm;
}

void ShaderManager::loadVertexShader(const std::string& key, const std::string &filePath)
{
    if (containsVertexShader(key))
    {
        auto msg = "Vertex shader with key '" + key + "' already exists!";
        throw std::runtime_error(msg.c_str());
    }

    auto vertexShader = std::make_unique<Shader>();
    if (!vertexShader->loadShaderFromFile(filePath, GL_VERTEX_SHADER))
    {
        auto msg = "Could not load vertex shader '" + filePath + "'!";
        throw std::runtime_error(msg);
    }

    _vertexShaderCache[key] = std::move(vertexShader);
}

void ShaderManager::loadFragmentShader(const std::string& key, const std::string &filePath)
{
    if (containsFragmentShader(key))
    {
        auto msg = "Fragment shader with key '" + key + "' already exists!";
        throw std::runtime_error(msg.c_str());
    }

    auto fragmentShader = std::make_unique<Shader>();
    if (!fragmentShader->loadShaderFromFile(filePath, GL_FRAGMENT_SHADER))
    {
        auto msg = "Could not load fragment shader '" + filePath + "'!";
        throw std::runtime_error(msg);
    }

    _fragmentShaderCache[key] = std::move(fragmentShader);
}

void ShaderManager::loadGeometryShader(const std::string& key, const std::string& filePath)
{
    if (containsGeometryShader(key))
    {
        auto msg = "Geometry shader with key '" + key + "' already exists!";
        throw std::runtime_error(msg.c_str());
    }

    auto geometryShader = std::make_unique<Shader>();
    if (!geometryShader->loadShaderFromFile(filePath, GL_GEOMETRY_SHADER))
    {
        auto msg = "Could not load geometry shader '" + filePath + "'!";
        throw std::runtime_error(msg);
    }

    _geometryShaderCache[key] = std::move(geometryShader);
}

bool ShaderManager::tryLoadGeometryShader(const std::string& key, const std::string& filePath)
{
    try
    {
        loadGeometryShader(key, filePath);
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}

const Shader& ShaderManager::getVertexShader(const std::string& key) const
{
    if (!containsVertexShader(key))
    {
        auto msg = "Attempting to get non-existing vertex shader with key '" + key + "'!";
        throw std::runtime_error(msg.c_str());
    }

    return *_vertexShaderCache.at(key);
}

const Shader& ShaderManager::getFragmentShader(const std::string& key) const
{
    if (!containsFragmentShader(key))
    {
        auto msg = "Attempting to get non-existing fragment shader with key '" + key + "'!";
        throw std::runtime_error(msg.c_str());
    }

    return *_fragmentShaderCache.at(key);
}

const Shader& ShaderManager::getGeometryShader(const std::string & key) const
{
    if (!containsGeometryShader(key))
    {
        auto msg = "Attempting to get non-existing geometry shader with key '" + key + "'!";
        throw std::runtime_error(msg.c_str());
    }

    return *_geometryShaderCache.at(key);
}

void ShaderManager::clearShaderCache()
{
    _vertexShaderCache.clear();
    _fragmentShaderCache.clear();
    _geometryShaderCache.clear();
}

bool ShaderManager::containsVertexShader(const std::string& key) const
{
    return _vertexShaderCache.count(key) > 0;
}

bool ShaderManager::containsFragmentShader(const std::string& key) const
{
    return _fragmentShaderCache.count(key) > 0;
}

bool ShaderManager::containsGeometryShader(const std::string & key) const
{
    return _geometryShaderCache.count(key) > 0;
}
