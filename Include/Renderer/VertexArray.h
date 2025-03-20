#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
    void Bind() const;
    void Unbind() const;
private:
    uint32_t m_VAO;
};