#pragma once

#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>

struct ProjectConfig
{
    std::string Name = "Untitled";

    std::filesystem::path AssetDirectory = "Assets";
    std::filesystem::path SceneDirectory = "Scenes";
    std::filesystem::path ShaderDirectory = "Shaders";
    std::filesystem::path MeshDirectory = "Meshes";

    static ProjectConfig LoadFromYAML(const std::filesystem::path& configPath)
    {
        YAML::Node config = YAML::LoadFile(configPath.string());

        ProjectConfig result;
        if (config["Name"])
            result.Name = config["Name"].as<std::string>();

        if (config["AssetDirectory"])
            result.AssetDirectory = config["AssetDirectory"].as<std::string>();

        if (config["SceneDirectory"])
            result.SceneDirectory = config["SceneDirectory"].as<std::string>();

        if (config["ShaderDirectory"])
            result.ShaderDirectory = config["ShaderDirectory"].as<std::string>();

        if (config["MeshDirectory"])
            result.MeshDirectory = config["MeshDirectory"].as<std::string>();

        return result;
    }
};
