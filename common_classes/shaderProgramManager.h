#pragma once

// STL
#include <string>
#include <map>
#include <memory>

// GLAD
#include <glad/glad.h>

// Project
#include "shaderProgram.h"

/**
 * Singleton class that manages and keeps track of all shader programs in the program.
 */
class ShaderProgramManager
{
public:
	/**
	 * Gets the one and only instance of the shader program manager.
	 */
	static ShaderProgramManager& getInstance();
	
	/**
	 * Creates new shader program and stores it with specified key.
	 *
	 * @param key  Key to store shader program with
	 *
	 * @return Shader program instance with specified key.
	 */
	ShaderProgram& createShaderProgram(const std::string& key);

	/**
	 * Retrieves shader program with a specified key.
	 *
	 * @param key  Key to get shader program from
	 *
	 * @return Shader program instance from a specified key.
	 */
	ShaderProgram& getShaderProgram(const std::string& key) const;

	/**
	 * Performs linkage of all existing shader programs.
	 */
	void linkAllPrograms();

	/**
	 * Deletes all the shader programs loaded and clears the shader program cache.
	 */
	void clearShaderProgramCache();

private:
	ShaderProgramManager() {} // Private constructor to make class singleton
	ShaderProgramManager(const ShaderProgramManager&) = delete; // No copy constructor allowed
	void operator=(const ShaderProgramManager&) = delete; // No copy assignment allowed

	/**
	 * Checks, if shader program with specified key exists.
	 *
	 * @param  key  Shader program key to check existence of
	 *
	 * @return True if shader program exists, or false otherwise.
	 */
	bool containsShaderProgram(const std::string& key) const;

	std::map<std::string, std::unique_ptr<ShaderProgram>> _shaderProgramCache; // Shader program cache - stores shader programs within their keys in std::map
};