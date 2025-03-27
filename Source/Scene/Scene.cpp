#include "Scene/Scene.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Geometry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Script.h"

void Scene::Load(AssetManager& assetManager)
{
    ShaderManager& shaderManager = assetManager.GetShaderManager();
    const ShaderProgram* program = shaderManager.LoadShaderProgram("Normal");
    MeshManager& meshManager = assetManager.GetMeshManager();
    const Mesh* mesh = meshManager.LoadMesh("ship.obj");

    std::vector<glm::vec3> triangle =
    {
        glm::vec3(1, 1, 1),
        glm::vec3(0, 1, 1),
        glm::vec3(0, 0, 1),
    };

    

    //entt::entity ship = m_Registry.create();
    //m_Registry.emplace<Geometry>(ship, mesh);
    //m_Registry.emplace<Material>(ship, program);
    //m_Registry.emplace<Transform>(ship);

    m_ActiveCamera = m_Registry.create();
    m_Registry.emplace<CameraData>(m_ActiveCamera, 45.0f, 1280, 720, 0.01f, 1000.0f);
    m_Registry.emplace<Transform>(m_ActiveCamera);
    Script& script = m_Registry.emplace<Script>(m_ActiveCamera);
    script.AddBehavior<CameraController>(m_ActiveCamera, m_Registry, 40.f, 1.f);
}