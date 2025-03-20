#include "Systems/AISystem.h"

void AISystem::Update(Scene& scene)
{
    for (auto& entity : scene.GetEntities())
    {
        entity.UpdateBehaviors();
    }
}
