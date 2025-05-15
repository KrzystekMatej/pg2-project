#pragma once
#include <string>
#include <unordered_map>
#include "Shader.h"

enum class UniformType
{
	Matrix4x4,
	Matrix3x3,
	Vec2,
	Vec3,
	Vec4,
	Float,
	Int32,
	UInt32
};

enum class ShaderType
{
	Normal,
	DirectPBR
};

class ShaderProgram
{
public:
	explicit ShaderProgram(ShaderType type);

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	~ShaderProgram();

	ShaderType GetType() const { return m_Type; }

	void AttachShader(const Shader& shader) const;
	void DetachShader(const Shader& shader) const;
	bool Link() const;
	void Use() const;
	void Unbind() const;
	int GetUniformLocation(const std::string& name) const;
	void SetUniform(UniformType uniformType, const std::string& name, const void* value) const;
private:
	bool CheckLinking() const;
	void Validate() const;

	uint32_t m_Id;
	ShaderType m_Type;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
