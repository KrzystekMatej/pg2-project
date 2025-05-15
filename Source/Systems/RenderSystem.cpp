#include "Systems/RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include "Assets/MaterialRegistry.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/PointLight.h"

void RenderSystem::Initialize(const AssetManager& assetManager) const
{
    const ShaderStorageBuffer* materialBuffer = assetManager.GetRegistry<MaterialRegistry>()->GetMaterialBuffer();
    materialBuffer->BindBase();
}

void RenderSystem::Draw(Scene& scene, float aspectRatio) const
{
    entt::registry& registry = scene.GetRegistry();
    auto group = registry.group<Transform, Material, Mesh>();
    auto [cameraTransform, camera] = registry.get<Transform, Camera>(scene.GetActiveCameraHandle());
    glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 view = cameraTransform.GetViewMatrix();

    auto lightView = registry.view<Transform, PointLight>();

    for (auto&& [entity, transform, material, geometry] : group.each())
    {
    	glm::mat4 model = transform.GetLocalToWorldMatrix();
        glm::mat3 normal = transform.GetNormalMatrix();
        glm::mat4 pv = projection * view;

        material.ShaderProgram->Use();
        material.ShaderProgram->SetUniform(UniformType::Matrix4x4, "modelMatrix", glm::value_ptr(model));
        material.ShaderProgram->SetUniform(UniformType::Matrix3x3, "normalMatrix", glm::value_ptr(normal));
        material.ShaderProgram->SetUniform(UniformType::Matrix4x4, "pvmMatrix", glm::value_ptr(pv * model));
        geometry.mesh->GetVertexArray().Bind();

        if (material.ShaderProgram->GetType() == ShaderType::DirectPBR)
        {
            material.ShaderProgram->SetUniform(UniformType::Vec3, "cameraPos", glm::value_ptr(cameraTransform.Position));

            uint32_t i = 0;
            for (auto [lightEntity, lightTransform, light] : lightView.each())
            {
                if (i < PointLight::s_MaxLightCount)
                {
                    material.ShaderProgram->SetUniform(UniformType::Vec3, std::format("lights[{}].position", i), glm::value_ptr(lightTransform.Position));
                    material.ShaderProgram->SetUniform(UniformType::Vec3, std::format("lights[{}].color", i), glm::value_ptr(light.Color));
                    i++;
                }
                else spdlog::error("Draw: Light limit exceeded!");
            }

            material.ShaderProgram->SetUniform(UniformType::UInt32, "lightCount", &i);
        }


        glDrawElements(GL_TRIANGLES, geometry.mesh->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
