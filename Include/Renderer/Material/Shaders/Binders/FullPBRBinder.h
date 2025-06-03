#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include "ShaderBinder.h"
#include "Renderer/Material/Texture.h"

class FullPBRBinder : public ShaderBinder
{
public:
    FullPBRBinder(const Texture* irradianceMap, const Texture* prefilterMap, const Texture* brdfTable)
        : m_IrradianceMap(irradianceMap), m_PrefilterMap(prefilterMap), m_BrdfTable(brdfTable) {}


    void Bind(const ShaderProgram& program, const DrawContext& ctx, const Transform& transform, const MeshHandle& mesh) const override
    {
        glm::mat4 model = transform.GetLocalToWorldMatrix();
        glm::mat3 normal = transform.GetNormalMatrix();
        glm::mat4 pv = ctx.Projection * ctx.View;

        program.SetMatrix4x4("modelMatrix", glm::value_ptr(model));
        program.SetMatrix3x3("normalMatrix", glm::value_ptr(normal));
        program.SetMatrix4x4("pvmMatrix", glm::value_ptr(pv * model));
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
        program.SetInt32("irradianceMap", 0);
        program.SetInt32("prefilterMap", 1);
        program.SetInt32("brdfTable", 2);
        Texture::Activate(0);
        m_IrradianceMap->Bind();
        Texture::Activate(1);
        m_PrefilterMap->Bind();
        Texture::Activate(2);
        m_BrdfTable->Bind();
    }

private:
    const Texture* m_IrradianceMap;
    const Texture* m_PrefilterMap;
    const Texture* m_BrdfTable;
};