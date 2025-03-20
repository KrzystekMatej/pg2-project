#include "Systems/RenderSystem.h"
#include "ECS/Components/Transform.h"

void RenderSystem::Update(const Scene& scene)
{
    for (const auto& entity : scene.GetEntities())
    {
        /*const MeshComponent* mesh = entity.GetComponent<MeshComponent>();
        const TransformComponent* transform = entity.GetComponent<TransformComponent>();

        if (mesh && transform)
        {
            Renderer::Draw(mesh->MeshData, transform->Position);
        }*/
    }
}
