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

void ShaderManager::clearShaderCache()
{
	_vertexShaderCache.clear();
	_fragmentShaderCache.clear();
}

bool ShaderManager::containsVertexShader(const std::string& key) const
{
	return _vertexShaderCache.count(key) > 0;
}

bool ShaderManager::containsFragmentShader(const std::string& key) const
{
	return _fragmentShaderCache.count(key) > 0;
}
