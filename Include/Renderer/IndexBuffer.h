#pragma once
#include <cstdint>

class IndexBuffer
{
public:
    IndexBuffer(const void* data, uint32_t count);

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;
    uint32_t GetCount() const { return m_Count; }

private:
    uint32_t m_IBO = 0;
    uint32_t m_Count = 0;
};
