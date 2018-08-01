#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "event.h"
#include "event_manager_interface.h"
#include "game_system_interface.h"
#include <memory>
#include <queue>
#include <vector>

typedef std::queue<std::shared_ptr<Event>> EventQueue;
typedef std::vector<GameSystemInterface*> Handlers;
typedef Handlers::iterator HandlersIter;

class EventManager : public EventManagerInterface {
public:
    void registerHandler(GameSystemInterface* system) {
        m_handlers.push_back(system);
    }

    void raiseEvent(Event* event) {
        m_events.push(std::shared_ptr<Event>(event));
    }

    void processEvents();

private:
    EventQueue m_events;
    Handlers m_handlers;
};

#endif
