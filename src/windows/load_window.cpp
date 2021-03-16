#include "load_window.h"
#include "../core/file_loader.h"
#include "../widgets/frame.h"
#include "../widgets/label.h"
#include "../widgets/listbox.h"
#include "../widgets/progress_bar.h"
#include "../widgets/tab.h"
#include "../widgets/text_entry.h"
#include "map_window.h"
#include <experimental/filesystem>
#include <iostream>
#include <thread>

namespace {
const std::string savePath = "./data/saves/";
}

void LoadWindow::setup() {
    setTitle("Save Game");
    setEscapeBehaviour(Window::EscapeBehaviour::CloseWindow);
    setHeight(15);
    setWidth(30);
}

void LoadWindow::registerWidgets() {
    createWidget<ListBox>("lstFiles", 1, 1)
        ->setItemSelectedCallback([=](ListBox* b) {
            this->loadState(b->getSelectedItem().getText());
        })
        ->setHeight(8);

    createWidget<Label>("lblStatus", 1, 1)
        ->setText("")
        ->setHorizontalAlign(Widget::HorizontalAlign::Left)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);
    createWidget<ProgressBar>("pgbProgress", 11, 2)
        ->setWidth(getWidth() - 14)
        ->setHorizontalAlign(Widget::HorizontalAlign::Left)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);
    createWidget<Label>("lblProgress", 1, 2)
        ->setText("Progress: ")
        ->setHorizontalAlign(Widget::HorizontalAlign::Left)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);

    createWidget<Label>("lblPlay", 1, 1)
        ->setText("Play!")
        ->setCommandChar(1)
        ->setIgnoreCommandCharCase(true)
        ->setCommandCharCallback([=](Label* l) {
            events()->fire<ReplaceAllWindowsEvent<MapWindow>>();
        })
        ->setHorizontalAlign(Widget::HorizontalAlign::Right)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom)
        ->setVisible(false);

    loadFiles();
}

void LoadWindow::loadFiles() {
    namespace fs = std::experimental::filesystem;

    auto lstFiles = getWidget<ListBox>("lstFiles");
    for (auto& filename : fs::directory_iterator(savePath)) {
        ListBoxItem item;
        item.setText(filename.path().filename().generic_string());
        lstFiles->addItem(item);
    }
}

void LoadWindow::loadState(const std::string& filename) {
    getWidget<ListBox>("lstFiles")->setSensitive(false);

    Label* lblStatus = getWidget<Label>("lblStatus");
    ProgressBar* pgbProgress = getWidget<ProgressBar>("pgbProgress");

    lblStatus->setText("Loading...");

    // Kick off background load
    std::thread update([=]() {
        FileLoader loader(this->map(), this->entities(), this->components());
        loader.setStatusCallback([=](unsigned int current, unsigned int max,
                                     const std::string& status) {
            lblStatus->setText(status);
            pgbProgress->setMaxValue(max)->setValue(current);
            lblStatus->setText(status);
        });
        loader.loadState(savePath + filename);
        lblStatus->setText("Loaded!");
        getWidget<Label>("lblPlay")->setVisible(true);
    });
    update.detach();
}
