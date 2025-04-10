#pragma once
#include <cstdint>

class VertexBuffer
{
public:
    VertexBuffer() : m_Id(0) {}
    VertexBuffer(const void* data, size_t size);

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

private:
    uint32_t m_Id;
};
