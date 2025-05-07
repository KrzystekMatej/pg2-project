#include <glad/gl.h>
#include "Renderer/ShaderProgram.h"
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
		glDeleteProgram(m_Id);
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

void ShaderProgram::SetUniform(UniformType uniformType, const std::string& name, const void* value) const
{
	int location = GetUniformLocation(name);
	if (location == -1) return;

	switch (uniformType)
	{
	case UniformType::Int32:
		glUniform1i(location, *static_cast<const GLint*>(value));
		break;
	case UniformType::Float:
		glUniform1f(location, *static_cast<const GLfloat*>(value));
		break;
	case UniformType::Vec2:
		glUniform2fv(location, 1, static_cast<const GLfloat*>(value));
		break;
	case UniformType::Vec3:
		glUniform3fv(location, 1, static_cast<const GLfloat*>(value));
		break;
	case UniformType::Vec4:
		glUniform4fv(location, 1, static_cast<const GLfloat*>(value));
		break;
	case UniformType::Matrix3x3:
		glUniformMatrix3fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(value));
		break;
	case UniformType::Matrix4x4:
		glUniformMatrix4fv(location, 1, GL_FALSE, static_cast<const GLfloat*>(value));
		break;
	default:
		spdlog::error("Unknown uniform type passed to SetUniform()");
		break;
	}
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