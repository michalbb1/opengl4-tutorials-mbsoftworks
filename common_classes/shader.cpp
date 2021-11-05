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

    std::vector<const char*> programSource;
    for(const auto& line : fileLines) {
        programSource.push_back(line.c_str());
    }

    // Create and compile shader
    shaderID_ = glCreateShader(shaderType);
    glShaderSource(shaderID_, static_cast<GLsizei>(fileLines.size()), programSource.data(), nullptr);
    glCompileShader(shaderID_);

    // Get and check the compilation status
    GLint compilationStatus;
    glGetShaderiv(shaderID_, GL_COMPILE_STATUS, &compilationStatus);
    if(compilationStatus == GL_FALSE)
    {
        std::cerr << "Error! Shader file " << fileName << " wasn't compiled!";

        // Get length of the error log first
        GLint logLength;
        glGetShaderiv(shaderID_, GL_INFO_LOG_LENGTH, &logLength);

        // If there is some log, then retrieve it and output extra information
        if (logLength > 0)
        {
            GLchar* logMessage = new GLchar[logLength];
            glGetShaderInfoLog(shaderID_, logLength, nullptr, logMessage);
            std::cerr << " The compiler returned: " << std::endl << std::endl << logMessage;
            delete[] logMessage;
        }

        std::cerr << std::endl;
        return false;
    }

    shaderType_ = shaderType;
    isCompiled_ = true;
    return true;
}

bool Shader::isCompiled() const
{
    return isCompiled_;
}

void Shader::deleteShader()
{
    if (shaderID_ == 0) {
        return;
    }

    std::cout << "Deleting shader with ID " << shaderID_ << std::endl;
    glDeleteShader(shaderID_);
    isCompiled_ = false;
    shaderID_ = 0;
}

GLuint Shader::getShaderID() const
{
    return shaderID_;
}

GLenum Shader::getShaderType() const
{
    return shaderType_;
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
    auto slashCharacter = '/';
    auto normFileName = string_utils::normalizeSlashes(fileName, slashCharacter);

    size_t slashIndex = -1;
    for (auto i = static_cast<int>(fileName.size()) - 1; i >= 0; i--)
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
    auto isInsideIncludePart = false;

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
