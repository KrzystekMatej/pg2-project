#include "Renderer/VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, size_t size)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_VBO(other.m_VBO)
{
    other.m_VBO = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = other.m_VBO;
        other.m_VBO = 0;
    }
    return *this;
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
