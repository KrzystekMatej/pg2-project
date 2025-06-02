#include <glad/glad.h>
#include "Renderer/Buffers/RenderBuffer.h"

RenderBuffer::RenderBuffer()
{
    glGenRenderbuffers(1, &m_Id);
}

RenderBuffer::RenderBuffer(uint32_t internalFormat, uint32_t width, uint32_t height)
{
    glGenRenderbuffers(1, &m_Id);
    Bind();
    Storage(internalFormat, width, height);
}

RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
    : m_Id(other.m_Id), m_InternalFormat(other.m_InternalFormat),
    m_Width(other.m_Width), m_Height(other.m_Height)
{
    other.m_Id = 0;
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept
{
    if (this != &other)
    {
        if (m_Id) glDeleteRenderbuffers(1, &m_Id);
        m_Id = other.m_Id;
        m_InternalFormat = other.m_InternalFormat;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        other.m_Id = 0;
    }
    return *this;
}

RenderBuffer::~RenderBuffer()
{
    if (m_Id) glDeleteRenderbuffers(1, &m_Id);
}

void RenderBuffer::Bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Id);
}

void RenderBuffer::Unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::Storage(uint32_t internalFormat, uint32_t width, uint32_t height)
{
    m_InternalFormat = internalFormat;
    m_Width = width;
    m_Height = height;
    glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, m_Width, m_Height);
}
