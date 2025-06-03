#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "ShaderBinder.h"
#include "Renderer/Material/Texture.h"
#include "ECS/Components/PointLight.h"

class FullPBRBinder : public ShaderBinder
{
public:
    FullPBRBinder(const Texture* irradianceMap, const Texture* prefilterMap, const Texture* brdfTable, bool enableShadows = true)
        : m_IrradianceMap(irradianceMap), m_PrefilterMap(prefilterMap), m_BrdfTable(brdfTable), m_EnableShadows(enableShadows) {}

    void Bind(const ShaderProgram& program, const DrawContext& ctx, const Transform& transform, const MeshHandle& mesh) const override
    {
        glm::mat4 model = transform.GetLocalToWorldMatrix();
        glm::mat4 pvm = ctx.Projection * ctx.View * model;
        glm::mat3 normal = transform.GetNormalMatrix();

        program.SetMatrix4x4("modelMatrix", model);
        program.SetMatrix3x3("normalMatrix", normal);
        program.SetMatrix4x4("pvmMatrix", pvm);
        program.SetVec3("cameraPosition", glm::value_ptr(ctx.CameraPosition));

        Texture::Activate(0);
        m_IrradianceMap->Bind();
        program.SetInt32("irradianceMap", 0);

        Texture::Activate(1);
        m_PrefilterMap->Bind();
        program.SetInt32("prefilterMap", 1);

        Texture::Activate(2);
        m_BrdfTable->Bind();
        program.SetInt32("brdfTable", 2);

        uint32_t i = 0;
        for (auto [entity, transform, light] : ctx.LightView.each())
        {
            if (i >= PointLight::s_MaxLightCount) break;

            program.SetVec3(std::format("lights[{}].position", i), glm::value_ptr(transform.Position));
            program.SetVec3(std::format("lights[{}].color", i), glm::value_ptr(light.Color));

            program.SetFloat(std::format("farPlanes[{}]", i), light.FarPlane);

            const Texture* depth = PointLight::s_DepthMaps[i].get();
            if (depth)
            {
                Texture::Activate(3 + i);
                depth->Bind();
                program.SetInt32(std::format("depthMaps[{}]", i), 3 + i);
            }
            i++;
        }

        program.SetUInt32("lightCount", i);
        program.SetInt32("enableShadows", m_EnableShadows ? 1 : 0);
        mesh.GetVertexArray().Bind();
    }

private:
    const Texture* m_IrradianceMap;
    const Texture* m_PrefilterMap;
    const Texture* m_BrdfTable;
    bool m_EnableShadows;
};
