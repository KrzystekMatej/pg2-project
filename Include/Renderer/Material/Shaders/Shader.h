#pragma once
#include <cstdint>
#include <string>
#include <filesystem>

class Shader
{
public:
    explicit Shader(uint32_t shaderType);

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    ~Shader();

    bool LoadSource(const std::filesystem::path& filePath);
    void SetSource(const std::string& source);
    bool Compile() const;
    bool CheckCompilation() const;
    void Attach(uint32_t programId) const;
    void Detach(uint32_t programId) const;

private:
    uint32_t m_ShaderType;
    uint32_t m_Id;
    std::string m_Source;
};