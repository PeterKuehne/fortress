#include "window_manager.h"
#include "splash_window.h"

void WindowManager::initialise (GameEngineInterface* engine) {
    m_engine = engine;

    Window* l_window = new SplashWindow();
    l_window->initialise(m_engine);
    pushWindow (l_window);
}

void WindowManager::pushWindow (WindowInterface* win) {
    m_windows.push_back (win);
    win->gainFocus();
    win->resize();
}

void WindowManager::popWindow () {
    if (m_windows.size() == 0) return;
    WindowInterface* win = m_windows.back();
    win->loseFocus();
    delete win;
    m_windows.pop_back();
}

void WindowManager::replaceWindow (WindowInterface* win) {
    int size = m_windows.size();
    for (int ii = 0; ii < size; ii++) {
        popWindow();
    }
    pushWindow (win);
}

WindowInterface* WindowManager::getActive()
{
    if (m_windows.size() == 0) return NULL;
    return m_windows.back();
}

void WindowManager::redraw()
{
    m_engine->getGraphics()->beginScreenUpdate();

    for (size_t ii = 0; ii < m_windows.size(); ii++) {
        m_windows[ii]->beforeRedraw();
        m_windows[ii]->redraw();
        m_windows[ii]->afterRedraw();
    }

    m_engine->getGraphics()->endScreenUpdate();

}

void WindowManager::resize ()
{
    for (size_t ii = 0; ii < m_windows.size(); ii++) {
        m_windows[ii]->resize();
    }
}

void WindowManager::update()
{
    for (size_t ii = 0; ii < m_windows.size(); ii++) {
        m_windows[ii]->update();
    }
}