#pragma once

#include "window.h"

class GameOverWindow : public Window {
public:
    GameOverWindow(bool win) : m_win(win){};
    void setup();
    void registerWidgets();

private:
    bool m_win;
};
