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
	Int32
};

class ShaderProgram
{
public:
	explicit ShaderProgram();
	~ShaderProgram();

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

	uint32_t m_ProgramID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
