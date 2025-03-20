#include "Scene/Scene.h"
#include "ECS/Components/Material.h"
#include "ECS/Components/Geometry.h"
#include "ECS/Components/Transform.h"

void Scene::Initialize(AssetManager& assetManager)
{
    ShaderManager& shaderManager = assetManager.GetShaderManager();
    const ShaderProgram* program = shaderManager.LoadShaderProgram("Basic");
    MeshManager& meshManager = assetManager.GetMeshManager();
    const Mesh* mesh = meshManager.LoadMesh("ship.obj");


    m_Entities.emplace_back(1);
    m_Entities[0].AddComponent<Geometry>(mesh);
    m_Entities[0].AddComponent<Material>(program);
    m_Entities[0].AddComponent<Transform>();


}

Entity& Scene::AddEntity()
{
    return m_Entities.emplace_back(m_Entities.size());
}

std::vector<Entity>& Scene::GetEntities()
{
    return m_Entities;
}

const std::vector<Entity>& Scene::GetEntities() const
{
    return m_Entities;
}
