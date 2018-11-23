#include "interaction_window.h"
#include "../core/event_manager.h"
#include "../widgets/frame.h"
#include "../widgets/label.h"
#include "../widgets/listbox.h"
#include "dialog_window.h"
#include "inspection_window.h"

void InteractionWindow::setup() {
    setTitle("Interact");
    setEscapeBehaviour(Window::EscapeBehaviour::CloseWindow);
}

void InteractionWindow::registerWidgets() {
    const unsigned int descriptionWidth = 15;
    const unsigned int commandWidth = 10;
    const unsigned int windowHeight = 10;

    getWidget<Frame>("frmBase")->setMergeBorders();

    Frame* frmEntityList = createWidget<Frame>("frmEntityList", 0, 0);
    frmEntityList->setMargin()
        ->setBorder()
        ->setWidth(descriptionWidth)
        ->setHeight(windowHeight);

    ListBox* lstEntities =
        createWidget<ListBox>("lstEntities", 0, 0, frmEntityList);
    lstEntities->setItemSelectedCallback(std::bind(
        &InteractionWindow::listSelection, this, std::placeholders::_1));
    lstEntities->setWidthStretchMargin(0)->setHeightStretchMargin(0);

    createWidget<Label>("txtInspect", descriptionWidth, 1)
        ->setText("inspect")
        ->setCommandChar(1)
        ->setCommandCharCallback([&](Label* l) {
            ListBox* lstEntities = this->getWidget<ListBox>("lstEntities");
            EntityId entity = m_entities[lstEntities->getSelection()];

            auto inspectionArgs = std::make_shared<InspectionWindowArgs>();
            inspectionArgs->entity = entity;
            auto win = std::make_shared<InspectionWindow>();
            win->setArguments(inspectionArgs);
            getEngine()->getWindows()->registerWindow(win);
        })
        ->setSensitive(false);
    createWidget<Label>("txtOpen", descriptionWidth, 2)
        ->setText("open")
        ->setCommandChar(1)
        ->setCommandCharCallback([&](Label* l) {
            ListBox* lstEntities = this->getWidget<ListBox>("lstEntities");
            EntityId entity = m_entities[lstEntities->getSelection()];
            ComponentStore& store = m_components[lstEntities->getSelection()];

            if (store.open->open) {
                auto event = new CloseEntityEvent;
                event->entity = entity;
                getEngine()->raiseEvent(event);
            } else {
                auto event = new OpenEntityEvent;
                event->entity = entity;
                getEngine()->raiseEvent(event);
            }
            getEngine()->swapTurn();
        })
        ->setSensitive(false);
    createWidget<Label>("txtTalk", descriptionWidth, 3)
        ->setText("talk")
        ->setCommandChar(1)
        ->setCommandCharCallback([&](Label* l) {
            ListBox* lstEntities = this->getWidget<ListBox>("lstEntities");
            EntityId entity = m_entities[lstEntities->getSelection()];
            EntityId playerId = getEngine()->state()->player();
            getEngine()->raiseEvent(
                std::make_shared<StartConversationEvent>(playerId, entity));
        })
        ->setSensitive(false);
    createWidget<Label>("txtDrop", descriptionWidth, 4)
        ->setText("pickup")
        ->setCommandChar(1)
        ->setCommandCharCallback([&](Label* l) {
            ListBox* lstEntities = this->getWidget<ListBox>("lstEntities");
            EntityId entity = m_entities[lstEntities->getSelection()];
            EntityId playerId = getEngine()->state()->player();
            getEngine()->events()->raise(
                std::make_shared<PickupEquipmentEvent>(playerId, entity));
            getEngine()->swapTurn();
        })
        ->setSensitive(false);

    setHeight(windowHeight);
    setWidth(descriptionWidth + commandWidth);

    lstEntities->clearItems();
    for (EntityId entity : m_arguments->entities) {
        ComponentStore store;
        store.desc =
            getEngine()->state()->components()->get<DescriptionComponent>(
                entity);
        store.open =
            getEngine()->state()->components()->get<OpenableComponent>(entity);
        store.drop =
            getEngine()->state()->components()->get<DroppableComponent>(entity);
        store.npc =
            getEngine()->state()->components()->get<NpcComponent>(entity);

        ListBoxItem item;
        item.setText(store.desc ? store.desc->title : "<Unknown>");
        lstEntities->addItem(item);
        m_entities.push_back(entity);
        m_components.push_back(store);
    }
    lstEntities->setSelection(0);
}

void InteractionWindow::updateScreen() {
    ListBox* lstEntities = getWidget<ListBox>("lstEntities");
    Label* txtInspect = getWidget<Label>("txtInspect");
    Label* txtOpen = getWidget<Label>("txtOpen");
    Label* txtDrop = getWidget<Label>("txtDrop");
    Label* txtTalk = getWidget<Label>("txtTalk");

    ComponentStore& store = m_components[lstEntities->getSelection()];

    txtInspect->setSensitive(store.desc != nullptr);
    txtOpen->setSensitive(store.open != nullptr);
    txtDrop->setSensitive(store.drop != nullptr);
    txtTalk->setSensitive(store.npc == nullptr);

    if (store.desc) {
        setTitle(store.desc->title);
    }
    if (store.open) {
        txtOpen->setText(store.open->open ? "close" : "open");
    }
}

void InteractionWindow::listSelection(ListBox* box) { updateScreen(); }

void InteractionWindow::nextTurn() { updateScreen(); }