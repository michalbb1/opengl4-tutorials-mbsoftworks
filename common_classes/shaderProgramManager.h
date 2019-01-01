#pragma once

#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>

#include "shaderProgram.h"

class ShaderProgramManager
{
public:
	ShaderProgramManager(const ShaderProgramManager&) = delete;
	void operator=(const ShaderProgramManager&) = delete;

	/** \brief  Gets the one and only instance of the shader program manager.
	*   \return Shader program manager instance.
	*/
	static ShaderProgramManager& getInstance();
	
	/** \brief  Creates new shader program and stores it with specified key.
	*   \param  key Key to store shader program with
	*   \return Shader program instance with specified key.
	*/
	ShaderProgram& createShaderProgram(const std::string& key);

	/** \brief  Retrieves shader program with a specified key.
	*   \param  key Key to restore shader program from
	*   \return Shader program instance from a specified key.
	*/
	ShaderProgram& getShaderProgram(const std::string& key) const;

	/** \brief  Deletes all the shader prrograms loaded and clears the shader program cache. */
	void clearShaderProgramCache();

private:
	ShaderProgramManager() {}

	/** \brief  Checks, if shader program with specified key exists.
	*   \param  key Shader program key to check existence of
	*   \return True if shader program exists, or false otherwise.
	*/
	bool containsShaderProgram(const std::string& key) const;

	std::map<std::string, std::unique_ptr<ShaderProgram>> _shaderProgramCache; //!< Shader program cache - stores shader programs within their keys in std::map
};