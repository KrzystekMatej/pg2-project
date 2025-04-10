#pragma once
#include <ECS/Entity.h>

class Behavior
{
public:
    Behavior(Entity entity) : m_Entity(entity) {}

    virtual ~Behavior() = default;
    virtual void Awake() = 0;
    virtual void Update() = 0;
protected:
    Entity m_Entity;
};
