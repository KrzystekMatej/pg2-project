#include "Renderer/IndexBuffer.h"
#include <glad/glad.h>

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_IBO);
}

IndexBuffer::IndexBuffer(const void* data, uint32_t count) : m_Count(count)
{
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}