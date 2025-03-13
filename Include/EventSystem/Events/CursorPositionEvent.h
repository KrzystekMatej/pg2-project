#pragma once
#include "Event.h"

class CursorPositionEvent : public Event
{
public:
    CursorPositionEvent(double x, double y) : Event(Type::CursorPosition), m_X(x), m_Y(y) {}

    double GetX() const { return m_X; }
    double GetY() const { return m_Y; }
private:
    double m_X;
    double m_Y;
};
