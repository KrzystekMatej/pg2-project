#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <filesystem>

namespace Utils
{
    inline std::string ToLower(const std::string& str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }


    inline std::string RemoveFileExtension(const std::string& filename)
    {
        return std::filesystem::path(filename).stem().string();
    }
}