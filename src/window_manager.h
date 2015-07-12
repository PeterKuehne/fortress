#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "window_manager_interface.h"
#include "window_interface.h"
#include <vector>

class GameEngineInterface;

class WindowManager : public WindowManagerInterface {
public:
    void initialise (GameEngineInterface* engine);
    void addWindow (WindowInterface* win) { m_windows.push_back (win); }
    void popWindow ();
    void replaceWindow (WindowInterface* win) ;
    WindowInterface* getActive();

private:
    GameEngineInterface*            m_engine;
    std::vector<WindowInterface*>   m_windows;
};

#endif
