#pragma once
#include "Renderer/Buffers/Mesh/MeshHandle.h"
#include "Renderer/Material/Shaders/ShaderProgram.h"
#include "ECS/Components/Transform.h"
#include "Renderer/DrawContext.h"

class ShaderBinder
{
public:
	virtual void Bind(const ShaderProgram& program, const DrawContext& ctx, const Transform& transform, const MeshHandle& mesh) const = 0;
	virtual ~ShaderBinder() = default;
};