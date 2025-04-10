#pragma once

#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>

struct ProjectConfig
{
    std::string name = "Untitled";

    std::filesystem::path assetDirectory = "Assets";
    std::filesystem::path sceneDirectory = "Scenes";
    std::filesystem::path shaderDirectory = "Shaders";
    std::filesystem::path meshDirectory = "Meshes";
    std::filesystem::path textureDirectory = "Textures";

    static ProjectConfig LoadFromYAML(const std::filesystem::path& configPath)
    {
        YAML::Node config = YAML::LoadFile(configPath.string());

        ProjectConfig result;
        if (config["Name"])
            result.name = config["Name"].as<std::string>();

        if (config["AssetDirectory"])
            result.assetDirectory = config["AssetDirectory"].as<std::string>();

        if (config["SceneDirectory"])
            result.sceneDirectory = config["SceneDirectory"].as<std::string>();

        if (config["ShaderDirectory"])
            result.shaderDirectory = config["ShaderDirectory"].as<std::string>();

        if (config["MeshDirectory"])
            result.meshDirectory = config["MeshDirectory"].as<std::string>();

        if (config["TextureDirectory"])
            result.textureDirectory = config["TextureDirectory"].as<std::string>();

        return result;
    }
};
