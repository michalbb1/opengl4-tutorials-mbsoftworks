#pragma once

#include <vector>
#include <string>

namespace string_utils
{
	/** \brief Splits the string with the given delimiter and returns the  std::vector of tokens.
	*   \param s String to split
	*   \param t Delimiter
	*   \return std::vector of tokens.
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

	/** \brief Goes one directory up in a path.
	*   \param originalPath   Original path
	*   \param slashCharacter Slash character to use (/ or \)
	*   \return Path with one directory up.
	*/
	std::string upOneDirectory(const std::string& originalPath, char slashCharacter)
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

	/** \brief Normalizes slashes in a path, so that all slashes are consistent
	*   \param originalPath   Original path
	*   \param slashCharacter Slash character to use (/ or \)
	*   \return Original path with same slash character everywhere.
	*/
	std::string normalizeSlashes(const std::string& originalPath, char slashCharacter)
	{
		std::string result = originalPath;
		for (char& c : result)
		{
			if (c == '\\' || c == '/')
				c = slashCharacter;
		}

		return result;
	}
}