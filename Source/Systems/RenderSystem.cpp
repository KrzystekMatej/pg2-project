#include "Systems/RenderSystem.h"

#include "ECS/Components/Mesh.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"

void RenderSystem::Draw(Scene& scene) const
{
    entt::registry& registry = scene.GetRegistry();
    auto group = registry.group<Transform, Material, Mesh>();
    auto [cameraTransform, camera] = registry.get<Transform, Camera>(scene.GetActiveCamera());
    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = cameraTransform.GetViewMatrix();

    for (auto&& [entity, transform, material, geometry] : group.each())
    {
    	glm::mat4 model = transform.GetLocalToWorldMatrix();
        glm::mat3 normal = transform.GetNormalMatrix();
    	m_Renderer.Draw(material.ShaderProgram, geometry.mesh->GetVertexArray(), geometry.mesh->GetIndexBuffer(), projection * view, model, normal);
    }
}
