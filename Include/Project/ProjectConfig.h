#pragma once

#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>

struct ProjectConfig
{
    std::string Name = "Untitled";

    std::filesystem::path AssetDirectory = "Assets";
    std::filesystem::path sceneDirectory = "Scenes";
    std::filesystem::path shaderDirectory = "Shaders";
    std::filesystem::path meshDirectory = "Meshes";

    static ProjectConfig LoadFromYAML(const std::filesystem::path& configPath)
    {
        YAML::Node config = YAML::LoadFile(configPath.string());

        ProjectConfig result;
        if (config["Name"])
            result.Name = config["Name"].as<std::string>();

        if (config["AssetDirectory"])
            result.AssetDirectory = config["AssetDirectory"].as<std::string>();

        if (config["SceneDirectory"])
            result.sceneDirectory = config["SceneDirectory"].as<std::string>();

        if (config["ShaderDirectory"])
            result.shaderDirectory = config["ShaderDirectory"].as<std::string>();

        if (config["MeshDirectory"])
            result.meshDirectory = config["MeshDirectory"].as<std::string>();

        return result;
    }
};
