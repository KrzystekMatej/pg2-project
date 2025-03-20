#pragma once
#include <vector>

#include "Assets/AssetManager.h"
#include "ECS/Entity.h"

class Scene
{
public:
    void Initialize(AssetManager& assetManager);
    Entity& AddEntity();
    std::vector<Entity>& GetEntities();
    const std::vector<Entity>& GetEntities() const;

private:
    std::vector<Entity> m_Entities;
};
