#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Renderer/Material/Texture.h"
#include "Config.h"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;
	~ShaderProgram();

	void AttachShader(const Shader& shader) const;
	void DetachShader(const Shader& shader) const;
	bool Link() const;
	void Use() const;
	void Unbind() const;

	int GetUniformLocation(const std::string& name) const;

	void SetInt32(const std::string& name, int value) const;
	void SetUInt32(const std::string& name, unsigned int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const float* vec2) const;
	void SetVec3(const std::string& name, const float* vec3) const;
	void SetVec4(const std::string& name, const float* vec4) const;
	void SetMatrix3x3(const std::string& name, const glm::mat3& mat) const;
	void SetMatrix4x4(const std::string& name, const glm::mat4& mat) const;
	void SetMatrix4x4Array(const std::string& baseName, uint32_t count, const glm::mat4* data) const;
	void SetTextureSampler(const std::string& name, const Texture& texture) const;
private:
	bool CheckLinking() const;
	void Validate() const;

	uint32_t m_Id;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};