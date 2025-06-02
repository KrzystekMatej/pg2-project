#include <glad/glad.h>
#include "Renderer/Material/Shaders/ShaderProgram.h"
#include <spdlog/spdlog.h>

ShaderProgram::ShaderProgram()
{
	m_Id = glCreateProgram();
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: m_Id(other.m_Id), m_UniformLocationCache(std::move(other.m_UniformLocationCache))
{
	other.m_Id = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this != &other)
	{
		if (m_Id) glDeleteProgram(m_Id);
		m_Id = other.m_Id;
		m_UniformLocationCache = std::move(other.m_UniformLocationCache);
		other.m_Id = 0;
	}
	return *this;
}


ShaderProgram::~ShaderProgram()
{
	if (m_Id)
	{
		glDeleteProgram(m_Id);
		m_Id = 0;
	}
}

void ShaderProgram::AttachShader(const Shader& shader) const
{
	shader.Attach(m_Id);
}

void ShaderProgram::DetachShader(const Shader& shader) const
{
	shader.Detach(m_Id);
}

bool ShaderProgram::Link() const
{
	glLinkProgram(m_Id);
	if (!CheckLinking()) return false;

#ifdef DEBUG
	Validate();
#endif

	return true;
}

void ShaderProgram::Validate() const
{
	glValidateProgram(m_Id);

	int success;
	glGetProgramiv(m_Id, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		std::vector<char> log(1024);
		glGetProgramInfoLog(m_Id, log.size(), nullptr, log.data());
		spdlog::warn("Shader Program Validation Warning:\n{}", log.data());
	}
}

int ShaderProgram::GetUniformLocation(const std::string& name) const
{
	auto it = m_UniformLocationCache.find(name);
	if (it != m_UniformLocationCache.end())
	{
		return it->second;
	}

	int location = glGetUniformLocation(m_Id, name.c_str());
	if (location == -1)
	{
		spdlog::error("Uniform '{}' does not exist!", name);
	}

	m_UniformLocationCache[name] = location;
	return location;
}

void ShaderProgram::SetInt32(const std::string& name, int value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1i(location, value);
}

void ShaderProgram::SetUInt32(const std::string& name, unsigned int value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1ui(location, value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1f(location, value);
}

void ShaderProgram::SetVec2(const std::string& name, const float* vec2) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform2fv(location, 1, vec2);
}

void ShaderProgram::SetVec3(const std::string& name, const float* vec3) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform3fv(location, 1, vec3);
}

void ShaderProgram::SetVec4(const std::string& name, const float* vec4) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform4fv(location, 1, vec4);
}

void ShaderProgram::SetMatrix3x3(const std::string& name, const float* mat3) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniformMatrix3fv(location, 1, GL_FALSE, mat3);
}

void ShaderProgram::SetMatrix4x4(const std::string& name, const float* mat4) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, mat4);
}

void ShaderProgram::SetTextureSampler(const std::string& name, const Texture& texture) const
{
	int location = GetUniformLocation(name);
	if (location == -1) return;
	
	//glUniformHandleui64ARB(location, texture.GetBindlessHandle());

	GLuint64 handle = texture.GetBindlessHandle();
	GLuint low = static_cast<GLuint>(handle & 0xFFFFFFFF);
	GLuint high = static_cast<GLuint>(handle >> 32);

	glUniform2ui(location, low, high);
}

bool ShaderProgram::CheckLinking() const
{
	GLint success;
	glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint logLength;
		glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<char> log(logLength);
		glGetProgramInfoLog(m_Id, logLength, nullptr, log.data());

		spdlog::error("Shader Program Linking Failure:\n{}", log.data());
		return false;
	}
	return true;
}

void ShaderProgram::Use() const
{
	glUseProgram(m_Id);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}