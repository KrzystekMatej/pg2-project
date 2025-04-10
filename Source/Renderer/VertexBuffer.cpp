#include "Renderer/VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, size_t size)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_Id(other.m_Id)
{
    other.m_Id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = other.m_Id;
        other.m_Id = 0;
    }
    return *this;
}

VertexBuffer::~VertexBuffer()
{
	if (m_Id)
	{
		glDeleteBuffers(1, &m_Id);
		m_Id = 0;
	}
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
