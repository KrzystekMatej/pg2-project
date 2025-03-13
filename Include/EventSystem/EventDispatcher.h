#pragma once
#include "Events/Event.h"
#include <unordered_map>
#include <vector>
#include <functional>

class EventDispatcher
{
public:
    using EventCallback = std::function<void(const Event&)>;

    static void AddListener(Event::Type type, EventCallback callback);
    static void DispatchEvent(const Event& event);
private:
    static std::unordered_map<Event::Type, std::vector<EventCallback>> m_Listeners;
};
