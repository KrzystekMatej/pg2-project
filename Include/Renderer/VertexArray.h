#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(VertexBuffer&& vertexBuffer, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
private:
    uint32_t m_VAO;
    std::vector<VertexBuffer> m_VertexBuffers;
};