#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"

class MeshHandle
{
public:
    MeshHandle(VertexArray&& vertexArray, IndexBuffer&& indexBuffer)
        : m_VertexArray(std::move(vertexArray)), m_IndexBuffer(std::move(indexBuffer)) {
    }

    MeshHandle(const MeshHandle&) = delete;
    MeshHandle& operator=(const MeshHandle&) = delete;

    MeshHandle(MeshHandle&&) noexcept = default;
    MeshHandle& operator=(MeshHandle&&) noexcept = default;

    const VertexArray& GetVertexArray() const { return m_VertexArray; }
    const IndexBuffer& GetIndexBuffer() const { return m_IndexBuffer; }

private:
    VertexArray m_VertexArray;
    IndexBuffer m_IndexBuffer;
};