#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include "ShaderBinder.h"
#include "Renderer/Material/Texture.h"

class DirectPBRBinder : public ShaderBinder
{
public:
    void Bind(const ShaderProgram& program, const DrawContext& ctx, const Transform& transform, const MeshHandle& mesh) const override
    {
        glm::mat4 model = transform.GetLocalToWorldMatrix();
        glm::mat3 normal = transform.GetNormalMatrix();
        glm::mat4 pv = ctx.Projection * ctx.View;

        program.SetMatrix4x4("modelMatrix", model);
        program.SetMatrix3x3("normalMatrix", normal);
        program.SetMatrix4x4("pvmMatrix", pv * model);
        mesh.GetVertexArray().Bind();

        program.SetVec3("cameraPosition", glm::value_ptr(ctx.CameraPosition));

        uint32_t i = 0;
        for (auto [lightEntity, lightTransform, light] : ctx.LightView.each())
        {
            if (i < PointLight::s_MaxLightCount)
            {
                program.SetVec3(std::format("lights[{}].position", i), glm::value_ptr(lightTransform.Position));
                program.SetVec3(std::format("lights[{}].color", i), glm::value_ptr(light.Color));
                i++;
            }
            else spdlog::error("Draw: Light limit exceeded!");
        }

        program.SetUInt32("lightCount", i);
    }
};