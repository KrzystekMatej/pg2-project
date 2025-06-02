#include "Systems/RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include "Assets/MaterialRegistry.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/PointLight.h"
#include "Renderer/DrawContext.h"

void RenderSystem::Initialize(const AssetManager& assetManager)
{
    const ShaderStorageBuffer* materialBuffer = assetManager.GetRegistry<MaterialRegistry>()->GetMaterialBuffer();
    materialBuffer->BindBase();
    m_BackgroundTexture = assetManager.GetRegistry<TextureRegistry>()->GetAsset("background");
    m_BackgroundShader = assetManager.GetRegistry<ShaderRegistry>()->GetAsset("background");
}

void RenderSystem::Draw(Scene& scene, float aspectRatio)
{
    entt::registry& registry = scene.GetRegistry();
    auto [cameraTransform, camera] = registry.get<Transform, Camera>(scene.GetActiveCameraHandle());

    DrawContext ctx
    {
        .Projection = camera.GetProjectionMatrix(aspectRatio),
        .View = cameraTransform.GetViewMatrix(),
        .CameraPosition = cameraTransform.Position,
        .LightView = registry.view<Transform, PointLight>()
    };

    Renderer::DrawPass(registry, ctx);
    if (m_BackgroundShader && m_BackgroundTexture) Renderer::DiffuseBackgroundPass(*m_BackgroundShader, *m_BackgroundTexture, ctx.Projection, ctx.View);
}
