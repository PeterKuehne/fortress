#include "game_over_window.h"
#include "../core/game_engine.h"
#include "label.h"

void GameOverWindow::registerWidgets() {
    Label* l = createWidget<Label>("lblResult", 2, 2);
    EntityId* l_entity = static_cast<EntityId*>(getArgs());
    if (l_entity) {
        l->setText("You Win!");
    } else {
        l->setText("You Lose!");
    }
}

void GameOverWindow::setup() {
    setTitle("GAME OVER");
    setHeight(13);
    setWidth(5);

    setEscapeBehaviour(Window::EscapeBehaviour::QuitGame);
}
