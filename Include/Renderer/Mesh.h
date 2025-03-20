#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"

struct Mesh
{
    explicit Mesh(VertexArray&& vertexArray, IndexBuffer&& indexBuffer)
        : VertexArray(std::move(vertexArray)), IndexBuffer(std::move(indexBuffer)) {}

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    VertexArray VertexArray;
    IndexBuffer IndexBuffer;
};