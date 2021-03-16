#pragma once

#include "../core/window_manager.h"
#include "event_manager.h"
#include "graphics_interface.h"
#include "location.h"
#include "lua_manager.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

// class EventManager;
class GameSystemBase;

class GameEngine {
public:
    explicit GameEngine(std::shared_ptr<GraphicsInterface> a_graphics);
    ~GameEngine() = default;

    void initialise(void);

    std::shared_ptr<GraphicsInterface> graphics() { return m_graphics; }
    std::shared_ptr<EventManager> events() { return m_eventManager; }
    std::unique_ptr<WindowManager>& windows() { return m_windowManager; }
    std::shared_ptr<MapManager> map() { return m_map; }
    std::shared_ptr<EntityManager> entities() { return m_entities; }
    std::shared_ptr<ComponentManager> components() { return m_components; }

    void addSystem(GameSystemBase* a_system) { m_systems.push_back(a_system); }

private:
    void tick();
    void turn();

private:
    std::shared_ptr<GraphicsInterface> m_graphics = nullptr;
    std::shared_ptr<EventManager> m_eventManager = nullptr;
    std::unique_ptr<WindowManager> m_windowManager = nullptr;
    std::shared_ptr<MapManager> m_map = nullptr;
    std::shared_ptr<EntityManager> m_entities = nullptr;
    std::shared_ptr<ComponentManager> m_components = nullptr;
    std::unique_ptr<LuaManager> m_lua = nullptr;
    std::vector<GameSystemBase*> m_systems;
};
