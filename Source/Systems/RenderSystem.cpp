#include "Systems/RenderSystem.h"
#include "Renderer/RenderContext.h"

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

    RenderContext ctx
    {
        .Projection = camera.GetProjectionMatrix(aspectRatio),
        .View = cameraTransform.GetViewMatrix(),
        .CameraPosition = cameraTransform.Position,
        .LightView = registry.view<Transform, PointLight>()
    };

    for (auto&& [entity, transform, material, geometry] : group.each())
    {
        for (const ShaderPipeline& pipeline : material.pipelines)
        {
            pipeline.Activate(ctx, transform, *geometry.mesh);
        }
        glDrawElements(GL_TRIANGLES, geometry.mesh->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
