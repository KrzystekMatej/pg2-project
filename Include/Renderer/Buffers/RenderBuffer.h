#pragma once
#include <cstdint>

class RenderBuffer
{
public:
    RenderBuffer();
    RenderBuffer(uint32_t internalFormat, uint32_t width, uint32_t height);

    RenderBuffer(const RenderBuffer&) = delete;
    RenderBuffer& operator=(const RenderBuffer&) = delete;

    RenderBuffer(RenderBuffer&& other) noexcept;
    RenderBuffer& operator=(RenderBuffer&& other) noexcept;

    ~RenderBuffer();

    void Bind() const;
    void Unbind() const;
    void Storage(uint32_t internalFormat, uint32_t width, uint32_t height);
    uint32_t GetId() const { return m_Id; }
    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }

private:
    uint32_t m_Id = 0;
    uint32_t m_InternalFormat = 0;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
};
