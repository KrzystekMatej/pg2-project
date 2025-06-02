#pragma once
#include <vector>
#include <cstdint>

class RenderBuffer;

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    FrameBuffer(FrameBuffer&& other) noexcept;
    FrameBuffer& operator=(FrameBuffer&& other) noexcept;

    void Bind() const;
    void Unbind() const;
    void AttachTexture(uint32_t attachment, uint32_t textureTarget, uint32_t textureId, int level = 0) const;
    void AttachRenderBuffer(uint32_t attachment, const RenderBuffer& rb) const;
    bool IsComplete() const;

    uint32_t GetId() const { return m_Id; }

private:
    uint32_t m_Id = 0;
};
