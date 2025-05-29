#pragma once
#include "Renderer/Buffers/Mesh/MeshHandle.h"

struct Mesh
{
    explicit Mesh(const MeshHandle* mesh) : mesh(mesh) {}

    const MeshHandle* mesh;
};