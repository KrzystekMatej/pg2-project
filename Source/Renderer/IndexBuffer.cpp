#include "Renderer/IndexBuffer.h"
#include <glad/glad.h>

IndexBuffer::IndexBuffer(const void* data, uint32_t count) : m_Count(count)
{
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_IBO(other.m_IBO), m_Count(other.m_Count)
{
    other.m_IBO = 0;
    other.m_Count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_IBO);
        m_IBO = other.m_IBO;
        m_Count = other.m_Count;
        other.m_IBO = 0;
        other.m_Count = 0;
    }
    return *this;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_IBO);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}