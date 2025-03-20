#include "Renderer/Shader.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

Shader::Shader(uint32_t shaderType)
    : m_ShaderType(shaderType)
{
    m_ShaderID = glCreateShader(shaderType);
}

Shader::~Shader()
{
    glDeleteShader(m_ShaderID);
}

bool Shader::LoadSource(const std::string& filePath) const
{
    std::ifstream file(filePath);
    if (!file)
    {
        spdlog::error("Failed to open shader file: {}", filePath);
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    const char* src = sourceCode.c_str();
    glShaderSource(m_ShaderID, 1, &src, nullptr);

    return true;
}

void Shader::SetSource(const std::string& source) const
{
    const char* src = source.c_str();
    glShaderSource(m_ShaderID, 1, &src, nullptr);
}

bool Shader::Compile() const
{
    glCompileShader(m_ShaderID);
    return CheckCompilation();
}

bool Shader::CheckCompilation() const
{
    int success;
    glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
        GLchar log[2048];
        glGetShaderInfoLog(m_ShaderID, sizeof(log), nullptr, log);

        spdlog::error("Shader Compilation Failure:\n{}", log);
        return false;
    }
    return true;
}

void Shader::Attach(uint32_t shaderProgram) const
{
    glAttachShader(shaderProgram, m_ShaderID);
}

void Shader::Detach(uint32_t shaderProgram) const
{
    glDetachShader(shaderProgram, m_ShaderID);
}
