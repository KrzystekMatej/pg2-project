#pragma once
#include <algorithm>
#include <cctype>
#include <string>
#include <filesystem>

namespace Utils
{
    inline std::string ToLower(std::string str)
    {
        std::ranges::transform(str, str.begin(), [](unsigned char c) -> char { return std::tolower(c); });
        return str;
    }


    inline std::string RemoveFileExtension(const std::string& filename)
    {
        return std::filesystem::path(filename).stem().string();
    }

    inline const std::string& EmptyString()
    {
        static const std::string empty;
        return empty;
    }
}