#pragma once

#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>

#include "shader.h"

class ShaderManager
{
public:
	ShaderManager(const ShaderManager&) = delete;
	void operator=(const ShaderManager&) = delete;

	/** \brief  Gets the one and only instance of the shader manager.
	*   \return Shader manager instance.
	*/
	static ShaderManager& getInstance();

	/** \brief  Creates new vertex shader and stores it with specified key.
	*   \param  key      Key to store vertex shader with
	*   \param  filePath Path to vertex shader file
	*/
	void loadVertexShader(const std::string& key, const std::string &filePath);

	/** \brief  Creates new fragment shader and stores it with specified key.
	*   \param  key Key to store fragment shader with
	*/
	void loadFragmentShader(const std::string& key, const std::string &filePath);

	/** \brief  Retrieves vertex shader with a specified key.
	*   \param  key Key to restore vertex shader from
	*   \return Vertex shader instance from a specified key.
	*/
	const Shader& getVertexShader(const std::string& key) const;

	/** \brief  Retrieves fragment shader with a specified key.
	*   \param  key Key to restore fragment shader from
	*   \return Fragment shader instance from a specified key.
	*/
	const Shader& getFragmentShader(const std::string& key) const;

	/** \brief  Deletes all the shaders loaded and clears the shaders cache. */
	void clearShaderCache();

private:
	ShaderManager() {}

	/** \brief  Checks, if vertex shader with specified key exists.
	*   \param  key Vertex shader key to check existence of
	*   \return True if vertex shader exists, or false otherwise.
	*/
	bool containsVertexShader(const std::string& key) const;

	/** \brief  Checks, if fragment shader with specified key exists.
	*   \param  key Fragment shader key to check existence of
	*   \return True if fragment shader exists, or false otherwise.
	*/
	bool containsFragmentShader(const std::string& key) const;

	std::map<std::string, std::unique_ptr<Shader>> _vertexShaderCache; //!< Vertex shader cache - stores vertex shaders within their keys in std::map
	std::map<std::string, std::unique_ptr<Shader>> _fragmentShaderCache; //!< Fragment shader cache - stores fragment shaders within their keys in std::map
};