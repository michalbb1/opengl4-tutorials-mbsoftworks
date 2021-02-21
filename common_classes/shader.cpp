// STL
#include <sstream>
#include <iostream>
#include <fstream>

// Project
#include "shader.h"
#include "stringUtils.h"

Shader::~Shader()
{
    deleteShader();
}

bool Shader::loadShaderFromFile(const std::string& fileName, GLenum shaderType)
{
    std::vector<std::string> fileLines;
    std::set<std::string> filesIncludedAlready;

    if(!getLinesFromFile(fileName, fileLines, filesIncludedAlready))
        return false;

    const char** programSource = new const char*[fileLines.size()];
    for (size_t i = 0; i < fileLines.size(); i++)
        programSource[i] = fileLines[i].c_str();
    
    _shaderID = glCreateShader(shaderType);

    glShaderSource(_shaderID, static_cast<GLsizei>(fileLines.size()), programSource, nullptr);
    glCompileShader(_shaderID);

    delete[] programSource;

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

bool Shader::isLoaded() const
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

GLuint Shader::getShaderID() const
{
    return _shaderID;
}

GLenum Shader::getShaderType() const
{
    return _shaderType;
}

bool Shader::getLinesFromFile(const std::string& fileName, std::vector<std::string>& result, std::set<std::string>& filesIncludedAlready, bool isReadingIncludedFile) const
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

    size_t slashIndex = -1;
    for (int i = static_cast<int>(fileName.size()) - 1; i >= 0; i--)
    {
        if (fileName[i] == slashCharacter)
        {
            slashIndex = i;
            slashCharacter = fileName[i];
            break;
        }
    }

    startDirectory = fileName.substr(0, slashIndex + 1);

    // Get all lines from a file

    std::string line;

    bool isInsideIncludePart = false;

    while (std::getline(file, line))
    {
        line += "\n"; // getline does not keep newline character
        std::stringstream ss(line);
        std::string firstToken;
        ss >> firstToken;
        if (firstToken == "#include")
        {
            std::string includeFileName;
            ss >> includeFileName;
            if (includeFileName.size() > 0 && includeFileName[0] == '\"' && includeFileName[includeFileName.size() - 1] == '\"')
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

                const auto combinedIncludeFilePath = directory + sFinalFileName;
                if (filesIncludedAlready.find(combinedIncludeFilePath) == filesIncludedAlready.end())
                {
                    filesIncludedAlready.insert(combinedIncludeFilePath);
                    getLinesFromFile(directory + sFinalFileName, result, filesIncludedAlready, true);
                }
            }
        }
        else if (firstToken == "#include_part")
            isInsideIncludePart = true;
        else if (firstToken == "#definition_part")
            isInsideIncludePart = false;
        else if (!isReadingIncludedFile || (isReadingIncludedFile && isInsideIncludePart))
            result.push_back(line);
    }

    file.close();

    return true;
}
