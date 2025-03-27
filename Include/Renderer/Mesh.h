#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"

class Mesh
{
public:
    Mesh(VertexArray&& vertexArray, IndexBuffer&& indexBuffer)
        : m_VertexArray(std::move(vertexArray)), m_IndexBuffer(std::move(indexBuffer)) {
    }

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    const VertexArray& GetVertexArray() const { return m_VertexArray; }
    const IndexBuffer& GetIndexBuffer() const { return m_IndexBuffer; }

private:
    VertexArray m_VertexArray;
    IndexBuffer m_IndexBuffer;
};