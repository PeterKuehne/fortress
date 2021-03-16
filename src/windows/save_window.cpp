#include "save_window.h"
#include "../core/file_saver.h"
#include "../widgets/frame.h"
#include "../widgets/label.h"
#include "../widgets/listbox.h"
#include "../widgets/progress_bar.h"
#include "../widgets/tab.h"
#include "../widgets/text_entry.h"
#include <experimental/filesystem>
#include <iostream>
#include <thread>

namespace {
const std::string savePath = "./data/saves/";
}

void SaveWindow::setup() {
    setTitle("Save Game");
    setEscapeBehaviour(Window::EscapeBehaviour::CloseWindow);
    setHeight(15);
    setWidth(30);
}

void SaveWindow::registerWidgets() {
    getWidget<Frame>("frmBase")->setMergeBorders();

    Tab* tab = createWidget<Tab>("tabSaveLocation", 1, 1);

    tab->setPageSwitchCallback([=](Tab* t) {
        //
    });
    Frame* createTab = tab->addPage("Create")->getFrame();
    createTab->setMargin(0);
    createWidget<Label>("lblCreate", 0, 0, createTab)->setText("Filename:");
    createWidget<TextEntry>("entFilename", 10, 0, createTab)
        ->setEnterCallback([&](TextEntry* e) {
            std::string filename;
            filename.append(e->getText());
            filename.append(e->getSuffix());
            this->saveState(filename);
        })
        ->setSuffix(std::string(".yaml"))
        ->setWidthStretchMargin(0)
        ->setHeightStretchMargin(0);

    createWidget<Label>("lblProgress", 1, 2)
        ->setText("Progress: ")
        ->setHorizontalAlign(Widget::HorizontalAlign::Left)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);
    createWidget<ProgressBar>("pgbProgress", 11, 2)
        ->setWidth(getWidth() - 14)
        ->setHorizontalAlign(Widget::HorizontalAlign::Left)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);

    createWidget<Label>("lblStatus", 1, 1)
        ->setText("")
        ->setHorizontalAlign(Widget::HorizontalAlign::Left)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);

    Frame* chooseTab = tab->addPage("Choose")->getFrame();
    chooseTab->setMargin(0);

    createWidget<ListBox>("lstFiles", 0, 0, chooseTab)
        ->setItemSelectedCallback([=](ListBox* b) {
            this->saveState(b->getSelectedItem().getText());
        })
        ->setHeight(8);
    loadFiles();
}

void SaveWindow::loadFiles() {
    namespace fs = std::experimental::filesystem;

    auto lstFiles = getWidget<ListBox>("lstFiles");
    for (auto& filename : fs::directory_iterator(savePath)) {
        ListBoxItem item;
        item.setText(filename.path().filename().generic_string());
        lstFiles->addItem(item);
    }
}

void SaveWindow::saveState(const std::string& filename) {
    getWidget<Tab>("tabSaveLocation")->setSensitive(false);

    Label* lblStatus = getWidget<Label>("lblStatus");
    ProgressBar* pgbProgress = getWidget<ProgressBar>("pgbProgress");

    lblStatus->setText("Saving...");

    // Kick off background save
    std::thread update([=]() {
        FileSaver saver(this->map(), this->entities(), this->components());
        saver.setStatusCallback([=](unsigned int current, unsigned int max,
                                    const std::string& status) {
            lblStatus->setText(status);
            pgbProgress->setMaxValue(max)->setValue(current);
            lblStatus->setText(status);
        });
        saver.saveState(savePath + filename);
        lblStatus->setText("Saved!");
    });
    update.detach();
}
