#pragma once
#include "Renderer/MeshHandle.h"

struct Mesh
{
    explicit Mesh(const MeshHandle* mesh) : mesh(mesh) {}

    const MeshHandle* mesh;
};