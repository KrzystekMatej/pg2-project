#pragma once
#include "Renderer/RenderContext.h"
#include "Renderer/Buffers/Mesh/MeshHandle.h"
#include "Renderer/Material/Shaders/ShaderProgram.h"
#include "ECS/Components/Transform.h"

class ShaderBinder
{
public:
	virtual void Bind(const ShaderProgram& program, const RenderContext& ctx, const Transform& transform, const MeshHandle& mesh) const = 0;
	virtual ~ShaderBinder() = default;
};