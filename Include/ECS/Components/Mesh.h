#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"

struct Mesh
{
    explicit Mesh(VertexArray&& vertexArray, IndexBuffer&& indexBuffer)
        : vertexArray(std::move(vertexArray)), indexBuffer(std::move(indexBuffer)) {
    }

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    VertexArray vertexArray;
    IndexBuffer indexBuffer;
};