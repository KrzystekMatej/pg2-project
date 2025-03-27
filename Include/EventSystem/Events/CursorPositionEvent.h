#pragma once
#include <glm/glm.hpp>
#include "Event.h"

class CursorPositionEvent : public Event
{
public:
    CursorPositionEvent(double x, double y) : Event(Type::CursorPosition), m_Position(x, y) {}

    glm::vec2 GetPosition() const { return m_Position; }
private:
    glm::vec2 m_Position;
};
