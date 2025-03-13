#pragma once
#include "Event.h"

class FrameBufferSizeEvent : public Event
{
public:
    FrameBufferSizeEvent(int width, int height) : Event(Type::FrameBufferSize), m_Width(width), m_Height(height) {}

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    int m_Width;
    int m_Height;
};
