#include "gameengine.h"
#include <string>

#include "generator.h"
#include "event_manager.h"
#include "entity_manager.h"
#include "window_manager.h"
#include "movement_system.h"
#include "combat_system.h"
#include "sprite_system.h"

GameEngine* g_engine = 0;

static void keyDown (unsigned char key, int x, int y)
{
    g_engine->getWindows()->getActive()->keyDown (key);
}

static void keyUp (unsigned char key, int x, int y)
{
    g_engine->getWindows()->getActive()->keyUp (key);
}

static void display (void)
{
    g_engine->tick();
}

static void mouseClick (int button, int state, int x, int y)
{
    if (state) {
        g_engine->getWindows()->getActive()->mouseUp (x, y, button);
    } else {
        g_engine->getWindows()->getActive()->mouseDown (x, y, button);
    }
}

static void resize (int width, int height)
{
    g_engine->getGraphics()->updateScreenSize (width, height);
    g_engine->getWindows()->resize();
}

GameEngine::GameEngine (GraphicsInterface* a_graphics)
: m_tick (0)
, m_playerTurn (true)
, m_entityManager (0)
, m_eventManager (0)
, m_windowManager (0)
, m_graphics (a_graphics)
, m_generator (0)
{
    g_engine = this;
}

GameEngine::~GameEngine ()
{

}

void GameEngine::initialise ()
{
    // Create if not exist
    if (!m_windowManager) m_windowManager = new WindowManager();
    if (!m_eventManager)  m_eventManager  = new EventManager();
    if (!m_entityManager) m_entityManager = new EntityManager();
    if (!m_generator)     m_generator     = new Generator();

    // Initialise Managers
    m_windowManager->initialise (this);
    m_entityManager->initialise (this);
    m_eventManager->initialise  (this);

    // Initialise Map Generator
    m_generator->initialise (this);

    // Initialise Systems
    for (unsigned int ii = 0; ii < m_systems.size(); ii++) {
        m_systems[ii]->initialise (this);
        m_eventManager->registerHandler (m_systems[ii]);
    }

    m_graphics->setKeyboardFunc (keyDown);
    m_graphics->setKeyboardUpFunc (keyUp);
    m_graphics->setDisplayFunc  (display);
    m_graphics->setMouseFunc    (mouseClick);
    m_graphics->setResizeFunc   (resize);

}

void GameEngine::loadMap (const std::string& mapName)
{
    m_generator->mapHeight() = 50;
    m_generator->mapWidth() = 50;
    m_generator->numberOfRooms() = 1;

    m_generator->generate();
}

void GameEngine::tick ()
{
    m_tick++; // Move the engine on

    m_eventManager->processEvents();

    //Update Systems
    for (unsigned int ii = 0; ii < m_systems.size(); ii++) {
        m_systems[ii]->update();
    }

    getWindows()->redraw();
//    getWindows()->getActive()->beforeRedraw();
//    getWindows()->getActive()->redraw();
//    getWindows()->getActive()->afterRedraw();

    return;
}
