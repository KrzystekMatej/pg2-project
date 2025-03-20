#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include "Behaviors/Behavior.h"

class Entity
{
public:
    Entity() : m_ID(0) {}
    Entity(int id) : m_ID(id) {}

    Entity(Entity&& other) noexcept = default;
    Entity& operator=(Entity&& other) noexcept = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        m_Components[typeid(T)] = component;
        return *component;
    }

    template <typename T>
    T* GetComponent()
    {
        auto it = m_Components.find(typeid(T));
        return (it != m_Components.end()) ? static_cast<T*>(it->second.get()) : nullptr;
    }

    template <typename T, typename... Args>
    void AddBehavior(Args&&... args)
    {
        m_Behaviors.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void UpdateBehaviors()
    {
        for (auto& behavior : m_Behaviors)
        {
            behavior->Update(*this);
        }
    }


    uint32_t GetID() const { return m_ID; }

private:
    uint32_t m_ID;
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_Components;
    std::vector<std::unique_ptr<Behavior>> m_Behaviors;
};
