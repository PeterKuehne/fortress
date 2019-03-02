#include "dialog_window.h"
#include "../components/player_component.h"
#include "../core/component_manager.h"
#include "../core/entity_manager.h"
#include "../core/event_manager.h"
#include "../widgets/frame.h"
#include "../widgets/label.h"
#include "../widgets/listbox.h"

void DialogWindow::setup() {
    setTitle("Dialog");
    setHeight(15);
    setWidth(40);

    setEscapeBehaviour(Window::EscapeBehaviour::CloseWindow);
}

void DialogWindow::registerWidgets() {
    getWidget<Frame>("frmBase")->setMergeBorders()->setMargin();

    createWidget<Label>("lblResponse", 0, 0)->setText("Yeah?");

    auto frmQuestions = createWidget<Frame>("frmQuestions", 0, 0);
    frmQuestions->setBorder()
        ->setHeight(7)
        ->setWidthStretchMargin(0)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);

    auto lstQuestions =
        createWidget<ListBox>("lstQuestions", 1, 0, frmQuestions);
    lstQuestions->setHeightStretchMargin(0)
        ->setWidthStretchMargin(0)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);
    lstQuestions->setItemSelectedCallback([=](ListBox* l) {
        ListBoxItem item = l->getSelectedItem();
        events()->raise(
            std::make_shared<ChooseDialogOptionEvent>(item.getValue()));
        events()->raise(std::make_shared<EndTurnEvent>());
    });

    this->nextTurn();
}

void DialogWindow::nextTurn() {
    auto lstQuestions = getWidget<ListBox>("lstQuestions");
    auto lblResponse = getWidget<Label>("lblResponse");
    auto player = components()->getUnique<PlayerComponent>().component;

    lstQuestions->clearItems();
    for (unsigned int ii = 0; ii < player->dialogOptions.size(); ii++) {
        ListBoxItem item;
        item.setValue(ii + 1);
        item.setText(player->dialogOptions[ii]);
        lstQuestions->addItem(item);
    }
    lblResponse->setText(player->dialogText);
}

void DialogWindow::destroy() {
    events()->raise(std::make_shared<EndConversationEvent>());
}