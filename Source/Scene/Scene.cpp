#include "Scene/Scene.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Mesh.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Script.h"
#include "Assets/ShaderRegistry.h"
#include "ECS/Behaviors/CameraController.h"

static void BuildComplexEntity(const std::vector<const MeshHandle*>& meshes, Scene* scene, const ShaderProgram* program)
{
    Entity parentEntity(scene->GetRegistry().create(), scene);
    parentEntity.AddComponent<Transform>();

    for (const MeshHandle* mesh : meshes)
    {
        Entity childEntity(scene->GetRegistry().create(), scene);
        childEntity.AddComponent<Transform>(parentEntity);
        childEntity.AddComponent<Material>(program);
        childEntity.AddComponent<Mesh>(mesh);
    }
}

void Scene::Load(const Project& project, const Window& window, AssetManager& assetManager)
{
    const ShaderProgram* program = assetManager.GetRegistry<ShaderRegistry>()->LoadShaderProgram(project.GetShaderDirectory(), "Normal");
    std::vector<const MeshHandle*> meshes = assetManager.LoadObjFile(project.GetMeshDirectory(), "ship.obj");

    BuildComplexEntity(meshes, this, program);


    /*entt::entity cube = m_Registry.create();
    m_Registry.emplace<Mesh>(cube, meshes[0]);
    m_Registry.emplace<Material>(cube, program);
    m_Registry.emplace<Transform>(cube, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));*/

    m_ActiveCamera = m_Registry.create();

    float aspectRatio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());
    m_Registry.emplace<Camera>(m_ActiveCamera, glm::radians(45.f), aspectRatio, 0.01f, 1000.0f);
    m_Registry.emplace<Transform>(m_ActiveCamera, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Script& script = m_Registry.emplace<Script>(m_ActiveCamera);
    script.AddBehavior<CameraController>(Entity(m_ActiveCamera, this), 20.f, 0.01f, window);
}