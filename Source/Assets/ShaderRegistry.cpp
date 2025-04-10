#include "Assets/ShaderRegistry.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <filesystem>

const ShaderProgram* ShaderRegistry::LoadShaderProgram
(
    const std::filesystem::path& directoryPath,
    const std::string& shaderName
)
{
    if (const ShaderProgram* cached = GetAsset(shaderName)) return cached;

    std::unique_ptr<ShaderProgram> shaderProgram = std::make_unique<ShaderProgram>();

    std::vector<std::pair<std::string, uint32_t>> shaderTypes =
    {
        {"vertex.glsl", GL_VERTEX_SHADER},
        {"fragment.glsl", GL_FRAGMENT_SHADER},
        {"geometry.glsl", GL_GEOMETRY_SHADER}
    };

    std::vector<Shader> shaders;

    for (const auto& [fileName, shaderType] : shaderTypes)
    {
        std::filesystem::path fullPath = directoryPath / fileName;

        if (!exists(fullPath))
        {
            spdlog::warn("Shader file '{}' does not exist, skipping.", fullPath.string());
            continue;
        }

        Shader shader(shaderType);
        if (!shader.LoadSource(fullPath) || !shader.Compile())
        {
            spdlog::error("Failed to load or compile shader: {}", fullPath.string());
            return nullptr;
        }

        shaderProgram->AttachShader(shader);
        shaders.push_back(std::move(shader));
    }

    if (!shaderProgram->Link())
    {
        spdlog::error("Failed to link shader program '{}'", shaderName);
        return nullptr;
    }

    for (auto& shader : shaders)
    {
        shaderProgram->DetachShader(shader);
    }

    return AddAsset(shaderName, std::move(shaderProgram));
}