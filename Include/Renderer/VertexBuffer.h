#pragma once
#include <cstdint>

class VertexBuffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
private:
    uint32_t m_VBO;
};