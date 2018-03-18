#include "shader.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include "stringUtils.h"

bool Shader::loadShaderFromFile(const std::string& fileName, GLenum shaderType)
{
	std::vector<std::string> fileLines;

	if(!getLinesFromFile(fileName, fileLines))
		return false;

	const char** sProgram = new const char*[fileLines.size()];
	for (int i = 0; i < int(fileLines.size()); i++)
		sProgram[i] = fileLines[i].c_str();
	
	_shaderID = glCreateShader(shaderType);

	glShaderSource(_shaderID, fileLines.size(), sProgram, NULL);
	glCompileShader(_shaderID);

	delete[] sProgram;

	int compilationStatus;
	glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &compilationStatus);

	if(compilationStatus == GL_FALSE)
	{
		char infoLogBuffer[2048];
		int logLength;
		glGetShaderInfoLog(_shaderID, 2048, &logLength, infoLogBuffer);
		std::cout << "Error! Shader file " << fileName << " wasn't compiled! The compiler returned:" << std::endl << std::endl << infoLogBuffer << std::endl;
		return false;
	}
	_shaderType = shaderType;
	_isLoaded = true;

	return true;
}

bool Shader::isLoaded()
{
	return _isLoaded;
}

void Shader::deleteShader()
{
	if (!_isLoaded)
		return;

	_isLoaded = false;
	glDeleteShader(_shaderID);
}

bool Shader::getLinesFromFile(const std::string& fileName, std::vector<std::string>& vResult, bool isReadingIncludedFile)
{
	std::ifstream file(fileName);

	if (!file.good())
	{
		std::cout << "File " << fileName << " not found! (Have you set the working directory of the application to $(SolutionDir)bin/?)" << std::endl;
		return false;
	}

	std::string startDirectory;
	char slashCharacter = '/';
	auto normFileName = string_utils::normalizeSlashes(fileName, slashCharacter);

	int slashIndex = -1;
	for (auto i = fileName.size() - 1; i >= 0; i--)
	{
		if(fileName[i] == slashCharacter)
		{
			slashIndex = i;
			slashCharacter = fileName[i];
			break;
		}
	}

	startDirectory = fileName.substr(0, slashIndex+1);

	// Get all lines from a file

	std::string line;

	bool isInsideIncludePart = false;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string firstToken;
		ss >> firstToken;
		if(firstToken == "#include")
		{
			std::string includeFileName;
			ss >> includeFileName;
			if(includeFileName.size() > 0 && includeFileName[0] == '\"' && includeFileName[includeFileName.size()-1] == '\"')
			{
				includeFileName = string_utils::normalizeSlashes(includeFileName.substr(1, includeFileName.size() - 2), slashCharacter);
				std::string directory = startDirectory;
				std::vector<std::string> subPaths = string_utils::split(includeFileName, slashCharacter);
				std::string sFinalFileName = "";
				for (const std::string& subPath : subPaths)
				{
					if (subPath == "..")
						directory = string_utils::upOneDirectory(directory, slashCharacter);
					else
					{
						if (sFinalFileName.size() > 0)
							sFinalFileName += slashCharacter;
						sFinalFileName += subPath;
					}
				}
				getLinesFromFile(directory+sFinalFileName, vResult, true);
			}
		}
		else if(firstToken == "#include_part")
			isInsideIncludePart = true;
		else if(firstToken == "#definition_part")
			isInsideIncludePart = false;
		else if(!isReadingIncludedFile || (isReadingIncludedFile && isInsideIncludePart))
			vResult.push_back(line);
	}
	
	file.close();

	return true;
}

GLuint Shader::getShaderID()
{
	return _shaderID;
}

GLenum Shader::getShaderType()
{
	return _shaderType;
}