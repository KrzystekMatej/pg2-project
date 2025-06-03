#include "Systems/RenderSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include "Assets/MaterialRegistry.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/PointLight.h"
#include "Renderer/DrawContext.h"
#include "Assets/TextureFactory.h"

#include "Core/Window.h"

void RenderSystem::Initialize(const AssetManager& assetManager)
{
    const ShaderStorageBuffer* materialBuffer = assetManager.GetRegistry<MaterialRegistry>()->GetMaterialBuffer();
    materialBuffer->BindBase();
    m_BackgroundTexture = assetManager.GetRegistry<TextureRegistry>()->GetAsset("Background");
    m_BackgroundShader = assetManager.GetRegistry<ShaderRegistry>()->GetAsset("Background");
    m_ShadowShader = assetManager.GetRegistry<ShaderRegistry>()->GetAsset("ShadowMapping");

    constexpr uint32_t SHADOW_SIZE = 1024;
    constexpr GLint    INTERNAL = GL_DEPTH_COMPONENT24;

    for (uint32_t i = 0; i < PointLight::s_MaxLightCount; ++i)
    {
        if (!PointLight::s_DepthMaps[i])
        {
            PointLight::s_DepthMaps[i] = TextureFactory::CreateEmptyCubeMap
            (
                SHADOW_SIZE,
                INTERNAL,
                GL_DEPTH_COMPONENT,
                GL_FLOAT,
                GL_NEAREST, GL_NEAREST,
                false
            );
        }
    }
}

void RenderSystem::Draw(Scene& scene, const Window& window)
{
    entt::registry& registry = scene.GetRegistry();
    auto [cameraTransform, camera] = registry.get<Transform, Camera>(scene.GetActiveCameraHandle());
    DrawContext ctx
    {
        .Projection = camera.GetProjectionMatrix(window.GetAspectRatio()),
        .View = cameraTransform.GetViewMatrix(),
        .CameraPosition = cameraTransform.Position,
        .LightView = registry.view<Transform, PointLight>()
    };

    if (m_ShadowShader) Renderer::ShadowPass(*m_ShadowShader, registry, ctx);
    window.SetViewport();
    Renderer::DrawPass(registry, ctx);
    if (m_BackgroundShader && m_BackgroundTexture) Renderer::DiffuseBackgroundPass(*m_BackgroundShader, *m_BackgroundTexture, ctx.Projection, ctx.View);
}
