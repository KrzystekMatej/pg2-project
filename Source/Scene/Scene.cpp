#include "Scene/Scene.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Geometry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Script.h"

void Scene::Load(const Window& window, AssetManager& assetManager)
{
    ShaderManager& shaderManager = assetManager.GetShaderManager();
    const ShaderProgram* program = shaderManager.LoadShaderProgram("Normal");
    MeshManager& meshManager = assetManager.GetMeshManager();
    const Mesh* mesh = meshManager.LoadMesh("box.obj");


    entt::entity cube = m_Registry.create();
    m_Registry.emplace<Geometry>(cube, mesh);
    m_Registry.emplace<Material>(cube, program);
    m_Registry.emplace<Transform>(cube, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    m_ActiveCamera = m_Registry.create();

    float aspectRatio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
    m_Registry.emplace<CameraData>(m_ActiveCamera, glm::radians(45.f), aspectRatio, 0.01f, 100.0f);
    m_Registry.emplace<Transform>(m_ActiveCamera, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Script& script = m_Registry.emplace<Script>(m_ActiveCamera);
    script.AddBehavior<CameraController>(m_ActiveCamera, m_Registry, 5.f, 0.01f, window);
}