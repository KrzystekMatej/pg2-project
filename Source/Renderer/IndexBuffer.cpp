#include "Renderer/IndexBuffer.h"
#include <glad/glad.h>

IndexBuffer::IndexBuffer(const void* data, uint32_t count) : m_Count(count)
{
    glGenBuffers(1, &m_Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_Id(other.m_Id), m_Count(other.m_Count)
{
    other.m_Id = 0;
    other.m_Count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = other.m_Id;
        m_Count = other.m_Count;
        other.m_Id = 0;
        other.m_Count = 0;
    }
    return *this;
}

IndexBuffer::~IndexBuffer()
{
    if (m_Id)
    {
        glDeleteBuffers(1, &m_Id);
        m_Id = 0;
    }
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}