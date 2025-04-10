#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

class VertexArray
{
public:
    VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    ~VertexArray();

    void AddBuffer(VertexBuffer&& vertexBuffer, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
private:
    uint32_t m_Id;
    std::vector<VertexBuffer> m_VertexBuffers;
};