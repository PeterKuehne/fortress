#ifndef __GAMEENGINE_H__
#define __GAMEENGINE_H__

#include "game_engine_interface.h"
#include "graphics_interface.h"
#include "window_manager_interface.h"
#include "game_system_interface.h"
#include "event_manager_interface.h"
#include "location.h"
#include "game_state.h"

#include <string>
#include <vector>
#include <cstdlib>


class GameEngine : public GameEngineInterface {
public:
    GameEngine (GraphicsInterface* a_graphics);
    ~GameEngine ();

    void initialise (void);
    void tick (void);

    void quit() { m_graphics->terminate(); }
    bool isPlayerTurn() { return m_playerTurn; }
    void swapTurn();
    unsigned int getTurn() { return m_turn; }
    void setTurn (unsigned int turn) { m_turn = turn; }

    void raiseEvent (Event* event) { m_eventManager->raiseEvent (event); }

    unsigned long long getTick() { return m_tick; }

    GraphicsInterface* getGraphics() { return m_graphics; }

    void setWindowManager (WindowManagerInterface* a_manager) { m_windowManager = a_manager; }
    void setEventManager (EventManagerInterface* a_manager) { m_eventManager = a_manager; }
    WindowManagerInterface* getWindows() { return m_windowManager; }

    void addSystem (GameSystemInterface* a_system) { m_systems.push_back(a_system); }

    void addMessage (const Message& message) { m_messages.push_back (message); }
    void addMessage (const MessageType&, const std::string& message);
    std::vector<Message>& getMessages() { return m_messages; }

    GameState* state() { return m_state; }

private:
    unsigned long long  m_tick;
    bool                m_playerTurn;
    unsigned int        m_turn;

    EventManagerInterface*  m_eventManager;
    WindowManagerInterface* m_windowManager;
    GameState*              m_state;

    std::vector<GameSystemInterface*>   m_systems;

    GraphicsInterface*      m_graphics;

    std::vector<Message>    m_messages;
    unsigned int            m_depth = 0;
    unsigned int            m_area = 0;
};

#endif
