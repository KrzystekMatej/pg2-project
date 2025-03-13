#include "EventSystem/EventDispatcher.h"

std::unordered_map<Event::Type, std::vector<EventDispatcher::EventCallback>> EventDispatcher::m_Listeners;

void EventDispatcher::AddListener(Event::Type type, EventCallback callback)
{
    m_Listeners[type].push_back(std::move(callback));
}

void EventDispatcher::DispatchEvent(const Event& event)
{
    auto it = m_Listeners.find(event.GetType());
    if (it != m_Listeners.end())
    {
        for (auto& callback : it->second)
        {
            callback(event);
        }
    }
}