#include <glad/gl.h>
#include "Renderer/Material/Shaders/Shader.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

Shader::Shader(uint32_t shaderType)
    : m_ShaderType(shaderType)
{
    m_Id = glCreateShader(shaderType);
}

Shader::~Shader()
{
    if (m_Id)
    {
        glDeleteShader(m_Id);
        m_Id = 0;
    }
}

bool Shader::LoadSource(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath);
    if (!file)
    {
        spdlog::error("Failed to open shader file: {}", filePath.string());
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    SetSource(buffer.str());
    return true;
}

void Shader::SetSource(const std::string& source)
{
    m_Source = source;
    const char* src = m_Source.c_str();
    glShaderSource(m_Id, 1, &src, nullptr);
}

bool Shader::Compile() const
{
    glCompileShader(m_Id);
    return CheckCompilation();
}

bool Shader::CheckCompilation() const
{
    int success;
    glGetShaderiv(m_Id, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
        GLchar log[2048];
        glGetShaderInfoLog(m_Id, sizeof(log), nullptr, log);

        spdlog::error("Shader Compilation Failure:\n{}", log);

        GLint major = 0;
        GLint minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        spdlog::info("GL_VERSION during shader compile: {}.{}", major, minor);
        return false;
    }
    return true;
}

void Shader::Attach(uint32_t shaderProgram) const
{
    glAttachShader(shaderProgram, m_Id);
}

void Shader::Detach(uint32_t shaderProgram) const
{
    glDetachShader(shaderProgram, m_Id);
}
