#pragma once
#include "Renderer/Mesh.h"

struct Geometry
{
    explicit Geometry(const Mesh* mesh) : mesh(mesh) {}

    const Mesh* mesh;
};