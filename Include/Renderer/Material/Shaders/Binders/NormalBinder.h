#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "ShaderBinder.h"

class NormalBinder : public ShaderBinder
{
public:
    void Bind(const ShaderProgram& program, const DrawContext& ctx, const Transform& transform, const MeshHandle& mesh) const override
    {
        glm::mat4 model = transform.GetLocalToWorldMatrix();
        glm::mat3 normal = transform.GetNormalMatrix();
        glm::mat4 pv = ctx.Projection * ctx.View;
        program.SetMatrix4x4("modelMatrix", glm::value_ptr(model));
        program.SetMatrix3x3("normalMatrix", glm::value_ptr(normal));
        program.SetMatrix4x4("pvmMatrix", glm::value_ptr(pv * model));
        mesh.GetVertexArray().Bind();
    }
};