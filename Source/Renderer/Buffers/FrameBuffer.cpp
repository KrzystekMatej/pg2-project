#include <glad/glad.h>
#include "Renderer/Buffers/FrameBuffer.h"
#include "Renderer/Buffers/RenderBuffer.h"
#include <spdlog/spdlog.h>

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &m_Id);
}

FrameBuffer::~FrameBuffer()
{
    if (m_Id) glDeleteFramebuffers(1, &m_Id);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept : m_Id(other.m_Id)
{
    other.m_Id = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
    if (this != &other)
    {
        if (m_Id) glDeleteFramebuffers(1, &m_Id);
        m_Id = other.m_Id;
        other.m_Id = 0;
    }
    return *this;
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AttachTexture(uint32_t attachment, uint32_t textureTarget, uint32_t textureId, int level) const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureTarget, textureId, level);
}

void FrameBuffer::AttachRenderBuffer(uint32_t attachment, const RenderBuffer& rb) const
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb.GetId());
}

bool FrameBuffer::IsComplete() const
{
    Bind();
    const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!complete) spdlog::error("FrameBuffer id:{} is not complete!", m_Id);
    return complete;
}
