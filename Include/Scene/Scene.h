#pragma once
#include <entt/entt.hpp>

class SceneLoader;

class Scene
{
public:
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;
    ~Scene() = default;

    entt::registry& GetRegistry() { return m_Registry; }
    const entt::registry& GetRegistry() const { return m_Registry; }
    entt::entity GetActiveCameraHandle() const { return  m_CameraHandle; }
private:
    friend SceneLoader;
    entt::registry m_Registry;
    entt::entity m_CameraHandle;
};
