#include "Assets/ShaderManager.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <filesystem>

ShaderManager::~ShaderManager()
{
    m_ShaderCache.clear();
}

const ShaderProgram* ShaderManager::LoadShaderProgram(const std::string& shaderName)
{
    auto it = m_ShaderCache.find(shaderName);
    if (it != m_ShaderCache.end())
    {
        spdlog::info("Shader program '{}' loaded from cache.", shaderName);
        return &it->second;
    }

    ShaderProgram shaderProgram;

    std::vector<std::pair<std::string, uint32_t>> shaderTypes =
    {
        {"vertex.glsl", GL_VERTEX_SHADER},
        {"fragment.glsl", GL_FRAGMENT_SHADER},
        {"geometry.glsl", GL_GEOMETRY_SHADER}
    };

    std::vector<Shader> shaders;
    std::string fullPath = m_ShaderDirectory + "/" + shaderName;

    for (const auto& [fileName, shaderType] : shaderTypes)
    {
        std::string shaderPath = fullPath + "/" + fileName;

        if (!std::filesystem::exists(shaderPath))
        {
            spdlog::warn("Shader file '{}' does not exist, skipping.", shaderPath);
            continue;
        }

        Shader shader(shaderType);
        if (!shader.LoadSource(shaderPath) || !shader.Compile())
        {
            spdlog::error("Failed to load or compile shader: {}", shaderPath);
            return nullptr;
        }

        shaderProgram.AttachShader(shader);
        shaders.push_back(std::move(shader));
    }

    if (!shaderProgram.Link())
    {
        spdlog::error("Failed to link shader program '{}'", shaderName);
        return nullptr;
    }

    for (auto& shader : shaders)
    {
        shaderProgram.DetachShader(shader);
    }

    auto result = m_ShaderCache.emplace(shaderName, std::move(shaderProgram));
    return &result.first->second;
}

const ShaderProgram* ShaderManager::GetShaderProgram(const std::string& shaderName) const
{
    auto it = m_ShaderCache.find(shaderName);
    if (it != m_ShaderCache.end()) return &it->second;

    spdlog::warn("Shader program '{}' not found in cache.", shaderName);
    return nullptr;
}

void ShaderManager::DeleteShaderProgram(const std::string& shaderName)
{
    m_ShaderCache.erase(shaderName);
}
