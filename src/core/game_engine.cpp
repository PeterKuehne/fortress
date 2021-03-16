#include "game_engine.h"
#include "../components/player_component.h"
#include "../windows/splash_window.h"
#include "../windows/window.h"
#include "entity_manager.h"
#include "event_manager.h"
#include "game_system_base.h"
#include "map_manager.h"
#include "window_manager.h"
#include <string>

GameEngine::GameEngine(std::shared_ptr<GraphicsInterface> a_graphics)
    : m_graphics(a_graphics), m_eventManager(std::make_shared<EventManager>()),
      m_windowManager(std::make_unique<WindowManager>()),
      m_map(std::make_shared<MapManager>()),
      m_entities(std::make_shared<EntityManager>()),
      m_components(std::make_shared<ComponentManager>()) ,
      m_lua(std::make_unique<LuaManager>()) {
    //
}

void GameEngine::initialise() {
    m_lua->initialise(m_eventManager.get());

    m_eventManager->subscribe<QuitEvent>(
        [this](auto event) { this->graphics()->terminate(); });

    m_eventManager->subscribe<EndTurnEvent>([this](auto event) {
        auto player = this->components()->getUnique<PlayerComponent>();
        if (player.id) {
            player.component->turn += 1;
            player.component->playerTurn = !player.component->playerTurn;
        }
        this->turn();
    });

    m_eventManager->subscribe<RemoveEntityEvent>([this](auto event) {
        this->components()->removeAll(event.entity);
        this->entities()->destroyEntity(event.entity);
    });
    m_eventManager->subscribe<UpdateTileSizeEvent>([this](auto event) {
        unsigned int height =
            this->graphics()->getTileHeight() + event.adjustment;
        unsigned int width =
            this->graphics()->getTileWidth() + event.adjustment;
        this->graphics()->updateTileSize(width, height);
        this->events()->fire<ResizeWindowsEvent>();
    });

    // Initialise Managers
    windows()->initialise(graphics(), events(), components(), entities(),
                          map());

    entities()->initialise(events());

    // Initialise Systems
    for (unsigned int ii = 0; ii < m_systems.size(); ii++) {
        m_systems[ii]->initialise(events(), components(), entities(), map());
    }

    graphics()->setKeyDownFunc([this](unsigned char key, int x, int y) {
        this->windows()->getActive()->keyDown(key);
    });
    graphics()->setKeyUpFunc([this](unsigned char key, int x, int y) {
        this->windows()->getActive()->keyUp(key);
    });
    graphics()->setDisplayFunc([this]() { this->tick(); });
    graphics()->setMouseFunc([this](int button, int state, int x, int y) {
        if (state) {
            this->windows()->getActive()->mouseUp(x, y, button);
        } else {
            this->windows()->getActive()->mouseDown(x, y, button);
        }
    });
    graphics()->setResizeFunc([this](int width, int height) {
        this->graphics()->updateScreenSize(width, height);
        this->windows()->resize();
    });

    events()->fire<AddWindowEvent<SplashWindow>>();

    events()->fire<EndTurnEvent>();
}

void GameEngine::tick() {
    m_eventManager->processEvents();

    // Update Systems
    for (unsigned int ii = 0; ii < m_systems.size(); ii++) {
        m_systems[ii]->onTick();
    }

    windows()->nextTick();
    
    m_lua->nextTick();
}

void GameEngine::turn() {
    // Update Systems
    for (unsigned int ii = 0; ii < m_systems.size(); ii++) {
        m_systems[ii]->onTurn();
    }
    windows()->nextTurn();
    m_lua->nextTurn();
}
