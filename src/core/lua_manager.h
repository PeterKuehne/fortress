#pragma once
#include "event_manager.h"
#include <sol/sol.hpp>

class LuaManager {

public:
    void initialise(EventManager* events);
    void nextTick() { m_lua["next_tick"];}
    void nextTurn() { m_lua["next_turn"];}
private:
    EventManager* events() { return m_events; }
    void loadLuaScripts();

    EventManager* m_events = nullptr;
    sol::state m_lua;
};