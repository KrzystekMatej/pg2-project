#include "Systems/AISystem.h"
#include "ECS/Components/Script.h"

void AISystem::Update(Scene& scene) const
{
    entt::registry& registry = scene.GetRegistry();
    auto scriptsView = registry.view<Script>();

    for (auto&& [entity, script] : scriptsView.each())
    {
        for (Behavior* behavior : script.GetBehaviors())
        {
            behavior->Update();
        }
    }
}

