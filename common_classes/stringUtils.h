#pragma once

#include <vector>
#include <string>

namespace string_utils
{
	inline std::vector<std::string> split(std::string s, char t)
	{
		std::vector<std::string> res;
		while (true)
		{
			int pos = s.find(t);
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

	std::string upOneDirectory(const std::string& input, char slashCharacter)
	{
		bool isTrailingSlash = input.back() == slashCharacter;
		std::vector<std::string> subPaths = split(input, slashCharacter);
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

	std::string normalizeSlashes(const std::string& s, char slashCharacter)
	{
		std::string result = s;
		for (char& c : result)
		{
			if (c == '\\' || c == '/')
				c = slashCharacter;
		}

		return result;
	}
}