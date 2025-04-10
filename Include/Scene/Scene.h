#pragma once
#include <entt/entt.hpp>
#include "Assets/AssetManager.h"
#include "Core/Window.h"
#include "Project/Project.h"

class Scene
{
public:
    Scene() = default;
    void Load(const Project& project, const Window& window, AssetManager& assetManager);
    entt::registry& GetRegistry() { return m_Registry; }
    const entt::registry& GetRegistry() const { return m_Registry; }
    entt::entity GetActiveCamera() const { return  m_ActiveCamera; }
private:
    entt::registry m_Registry;
    entt::entity m_ActiveCamera;
};
