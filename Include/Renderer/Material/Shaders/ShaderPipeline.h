#pragma once
#include "Binders/ShaderBinder.h"
#include "Renderer/DrawContext.h"

struct ShaderPipeline
{
    const ShaderProgram* Program;
    std::shared_ptr<ShaderBinder> Binder;

    ShaderPipeline(const ShaderProgram* program, std::shared_ptr<ShaderBinder> binder)
        : Program(program), Binder(binder) {}

    void Activate(const DrawContext& ctx, const Transform& transform, const MeshHandle& mesh) const
    {
        Program->Use();
        Binder->Bind(*Program, ctx, transform, mesh);
    }
};