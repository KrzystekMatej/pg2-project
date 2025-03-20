#include "Scene/Scene.h"

void Scene::Initialize(const AssetManager& assetManager)
{
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
