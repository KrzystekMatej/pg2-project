#include <glad/gl.h>
#include "Renderer/Buffers/ShaderStorageBuffer.h"
#include <spdlog/spdlog.h>

ShaderStorageBuffer::ShaderStorageBuffer(uint32_t bindingIndex, size_t capacity, size_t stride)
	: m_BindingIndex(bindingIndex), m_Capacity(capacity), m_Stride(stride)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_Capacity * m_Stride, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingIndex, m_Id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	if (m_Id) glDeleteBuffers(1, &m_Id);
	m_Id = 0;
}

void ShaderStorageBuffer::Bind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Id);
}

void ShaderStorageBuffer::BindBase() const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingIndex, m_Id);
}

void ShaderStorageBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::SetData(const void* data, size_t size, size_t offset)
{
	if (offset + size > m_Capacity * m_Stride)
	{
		Resize(m_Capacity * 2);
		spdlog::info("ShaderStorageBuffer id:{}, bid: {} - resized: (new capacity = {})", m_Id, m_BindingIndex, m_Capacity);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Id);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::Resize(size_t newCapacity)
{
	if (newCapacity == m_Capacity) return;

	std::vector<uint8_t> oldData(m_Capacity * m_Stride);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Id);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, oldData.size(), oldData.data());

	m_Capacity = newCapacity;
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_Capacity * m_Stride, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, oldData.size(), oldData.data());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
