#pragma once

class Entity;

class Behavior
{
public:
    virtual ~Behavior() = default;
    virtual void Update(Entity& entity) = 0;
};
