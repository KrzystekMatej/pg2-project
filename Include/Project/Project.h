#pragma once

#include "ProjectConfig.h"

class Project
{
public:
    explicit Project(std::filesystem::path rootDirectory,  ProjectConfig config)
        : m_RootDirectory(std::move(rootDirectory)), m_Config(std::move(config)) {}

    static Project Load(const std::filesystem::path& configFilePath)
    {
        ProjectConfig config = ProjectConfig::LoadFromYAML(configFilePath);
        return Project(configFilePath.parent_path(), config);
    }

    const std::filesystem::path& GetRootDirectory() const
    {
        return m_RootDirectory;
    }

    std::filesystem::path GetAssetDirectory() const
    {
        return m_RootDirectory / m_Config.assetDirectory;
    }

    std::filesystem::path GetSceneDirectory() const
    {
        return GetAssetDirectory() / m_Config.sceneDirectory;
    }

    std::filesystem::path GetShaderDirectory() const
    {
        return GetAssetDirectory() / m_Config.shaderDirectory;
    }

    std::filesystem::path GetMeshDirectory() const
    {
        return GetAssetDirectory() / m_Config.meshDirectory;
    }

    std::filesystem::path GetTextureDirectory() const
    {
        return GetAssetDirectory() / m_Config.textureDirectory;
    }

    const ProjectConfig& GetConfig() const { return m_Config; }

private:
    std::filesystem::path m_RootDirectory;
    ProjectConfig m_Config;
};
