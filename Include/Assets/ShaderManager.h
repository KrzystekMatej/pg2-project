#pragma once
#include "Renderer/ShaderProgram.h"
#include <unordered_map>
#include <string>

class ShaderManager
{
public:
    explicit ShaderManager(const std::string& shaderDirectory) : m_ShaderDirectory(shaderDirectory) {}
    ~ShaderManager();

    const ShaderProgram* LoadShaderProgram(const std::string& shaderName);
    const ShaderProgram* GetShaderProgram(const std::string& shaderName) const;
    void DeleteShaderProgram(const std::string& shaderName);
private:
    std::string m_ShaderDirectory;
    std::unordered_map<std::string, ShaderProgram> m_ShaderCache;
};
