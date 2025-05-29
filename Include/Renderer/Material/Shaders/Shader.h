#pragma once
#include <cstdint>
#include <string>
#include <filesystem>

class Shader
{
public:
    explicit Shader(uint32_t shaderType);
    ~Shader();

    bool LoadSource(const std::filesystem::path& filePath);
    void SetSource(const std::string& source);
    bool Compile() const;
    bool CheckCompilation() const;
    void Attach(uint32_t shaderProgram) const;
    void Detach(uint32_t shaderProgram) const;

private:
    uint32_t m_ShaderType;
    uint32_t m_Id;
    std::string m_Source;
};