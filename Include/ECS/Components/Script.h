#pragma once
#include <vector>
#include <memory>
#include "ECS/Behaviors/Behavior.h"

class Script
{
public:
    Script() = default;
    ~Script()
    {
        for (auto* behavior : m_Behaviors)
        {
            delete behavior;
        }
        m_Behaviors.clear();
    }

    std::vector<Behavior*>& GetBehaviors() { return m_Behaviors; }

    template<typename TBehavior>
    TBehavior* GetBehavior()
    {
        static_assert(std::is_base_of_v<Behavior, TBehavior>);
        for (auto* behavior : m_Behaviors)
        {
            if (auto casted = dynamic_cast<TBehavior*>(behavior)) return casted;
        }
        return nullptr;
    }

    template<typename TBehavior>
    std::vector<TBehavior*> GetBehaviors()
    {
        static_assert(std::is_base_of_v<Behavior, TBehavior>);
        std::vector<TBehavior*> result;
        for (auto* behavior : m_Behaviors)
        {
            if (auto casted = dynamic_cast<TBehavior*>(behavior)) result.push_back(casted);
        }
        return result;
    }

    template<typename TBehavior, typename... Args>
    TBehavior* AddBehavior(entt::entity entity, entt::registry& registry, Args&&... args)
    {
        static_assert(std::is_base_of_v<Behavior, TBehavior>, "TBehavior must inherit from Behavior");

        auto* behavior = new TBehavior(std::forward<Args>(args)...);
        behavior->Awake(entity, registry);
        m_Behaviors.push_back(behavior);
        return behavior;
    }

    template<typename TBehavior>
    void RemoveBehavior()
    {
        static_assert(std::is_base_of_v<Behavior, TBehavior>);
        std::erase_if(
            m_Behaviors,
            [](Behavior* b)
            {
                if (dynamic_cast<TBehavior*>(b))
                {
                    delete b;
                    return true;
                }
                return false;
            }
        );
    }

    void RemoveBehavior(Behavior* ptr)
    {
        std::erase_if(
            m_Behaviors,
            [ptr](Behavior* b)
            {
                if (b == ptr)
                {
                    delete b;
                    return true;
                }
                return false;
            }
        );
    }
private:
    std::vector<Behavior*> m_Behaviors;
};
