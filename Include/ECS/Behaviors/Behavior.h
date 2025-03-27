#pragma once
#include <entt/entt.hpp>

class Behavior
{
public:
    virtual ~Behavior() = default;
    virtual void Awake(entt::entity entity, entt::registry& registry) = 0;
    virtual void Update(entt::entity entity, entt::registry& registry) = 0;
};
