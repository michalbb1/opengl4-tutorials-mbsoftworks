#pragma once

// STL
#include <vector>
#include <string>
#include <sstream>

namespace string_utils
{

/**
 * Splits given string with given delimiter and returns std::vector of tokens.
 *
 * @param s String to split
 * @param t Delimiter
 *
 * @return std::vector of tokens.
 */
inline std::vector<std::string> split(std::string s, char t)
{
    std::vector<std::string> res;
    while (true)
    {
        auto pos = s.find(t);
        if (pos == -1)
        {
            if (s.size() > 0)
                res.push_back(s);
            break;
        }
        std::string tp = s.substr(0, pos);
        if (tp.size() != 0)
            res.push_back(tp);
        s = s.substr(pos + 1, s.size() - pos - 1);
    }
    return res;
}

/**
 * Goes one directory up in a path.
 * 
 * @param originalPath   Original path
 * @param slashCharacter Slash character to use (/ or \)
 * 
 * @return Path with one directory up.
 */
inline std::string upOneDirectory(const std::string& originalPath, char slashCharacter)
{
    bool isTrailingSlash = originalPath.back() == slashCharacter;
    std::vector<std::string> subPaths = split(originalPath, slashCharacter);
    std::string result = "";
    for (size_t i = 0; i < subPaths.size() - 1; i++)
    {
        if (i > 0)
            result += slashCharacter;
        result += subPaths[i];
    }
    if (isTrailingSlash && result.size() > 0)
        result += slashCharacter;

    return result;
}

/**
 * Normalizes slashes in a path, so that all slashes are consistent.
 * 
 * @param originalPath   Original path
 * @param slashCharacter Slash character to use (/ or \)
 * 
 * @return Original path with same slash character everywhere.
 */
inline std::string normalizeSlashes(const std::string& originalPath, char slashCharacter)
{
    std::string result = originalPath;
    for (char& c : result)
    {
        if (c == '\\' || c == '/')
            c = slashCharacter;
    }

    return result;
}

inline std::string getDirectoryPath(const std::string& filePath)
{
    auto lastSlashIndex = filePath.find_last_of('\\');
    if (lastSlashIndex == std::string::npos) {
        lastSlashIndex = filePath.find_last_of('/');
    }

    if (lastSlashIndex == std::string::npos) {
        return "";
    }

    return filePath.substr(0, lastSlashIndex + 1);
}

inline std::string formatStringImpl(const char* s, std::stringstream& ss)
{
    ss << s;
    return ss.str();
}

template <typename T, typename... Args>
inline std::string formatStringImpl(const char* s, std::stringstream& ss, T arg, const Args&... args)
{
    std::string result;
    while (*s != 0)
    {
        if (*s == '{' && *(s + 1) == '}')
        {
            ss << arg;
            result = formatStringImpl(s + 2 * sizeof(char), ss, args...);
            break;
        }
        else ss << *s;
        s++;
    }
    return result;
}

template <typename... Args>
inline std::string formatString(const std::string& fmt, const Args&... args)
{
    std::stringstream ss;
    return formatStringImpl(fmt.c_str(), ss, args...);
}

} // namespace string_utils