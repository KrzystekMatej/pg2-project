#pragma once
#include "Event.h"

class KeyEvent : public Event
{
public:
    KeyEvent(int key, int mods) : Event(Type::Keyboard), m_Key(key), m_Mods(mods) {}

    int GetKey() const { return m_Key; }
    int GetMods() const { return m_Mods; }
private:
    int m_Key;
    int m_Mods;
};
