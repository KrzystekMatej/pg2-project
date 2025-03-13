#pragma once

class Event
{
public:
    enum class Type
    {
        FrameBufferSize,
        MouseButton,
        CursorPosition,
        Keyboard
    };

    explicit Event(Type type) : m_Type(type) {}

    Type GetType() const { return m_Type; }
private:
    Type m_Type;
};
