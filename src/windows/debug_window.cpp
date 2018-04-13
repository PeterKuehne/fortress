#include "debug_window.h"
#include "../core/game_engine.h"
#include "frame.h"
#include "listbox.h"
#include "text_entry.h"

#include <dirent.h>

const char CONSOLE_DIR[] = "./scripts/console";

void DebugWindow::setupLua()
{
    m_lua.setGameState(getEngine()->state());
    try
    {
        loadLuaScripts();
    }
    catch (const std::runtime_error &error)
    {
        Output line(error.what(), ERROR_COLOR);
        history.push_back(line);
    }
}

void DebugWindow::loadLuaScripts()
{
    DIR *directory = opendir(CONSOLE_DIR);
    if (directory == nullptr)
    {
        LOG(ERROR) << "Failed to open '"
                   << CONSOLE_DIR << "'"
                   << std::endl;
        throw std::runtime_error("Failed to open console directory");
    }
    struct dirent *file = nullptr;
    while ((file = readdir(directory)) != nullptr)
    {
        std::string filename(CONSOLE_DIR);
        filename.append("/").append(file->d_name);
        if (filename.find(".lua") != std::string::npos)
        {
            LOG(INFO) << "Loading lua script: " << filename << std::endl;
            try
            {
                m_lua.loadFile(filename);
                Output line(std::string("Loaded: ").append(filename), OUTPUT_COLOR);
                history.push_back(line);
            }
            catch (const std::runtime_error &error)
            {
                Output line(error.what(), ERROR_COLOR);
                history.push_back(line);
            }
        }
    }
    closedir(directory);
}

void DebugWindow::setup()
{
    setTitle("Debug Console");
    setFullscreen();
    setEscapeBehaviour(Window::EscapeBehaviour::CloseWindow);
    setupLua();
}

void DebugWindow::registerWidgets()
{
    getWidget<Frame>("frmBase")->setMergeBorders();
    Frame *frmOutput = createWidget<Frame>("frmOutput", 0, 0);
    frmOutput
        ->setBorder()
        ->setMargin()
        ->setWidthStretchMargin(0)
        ->setHeightStretchMargin(5);
    createWidget<ListBox>("lstOutput", 0, 0, frmOutput)
        ->setWidthStretchMargin(0)
        ->setHeightStretchMargin(0)
        ->setSensitive(false);
    Frame *frmEntry = createWidget<Frame>("frmEntry", 0, 0);
    frmEntry
        ->setBorder()
        ->setMargin()
        ->setWidthStretchMargin(0)
        ->setHeight(5)
        ->setVerticalAlign(Widget::VerticalAlign::Bottom);
    createWidget<TextEntry>("txtInput", 0, 0, frmEntry)
        ->setEnterCallback([&](TextEntry *e) {
            Output line;
            line.text = std::string(e->getText());
            line.color = COMMAND_COLOR;
            history.push_back(line);

            try
            {
                line.text = std::string(">>>").append(
                    m_lua.executeCommand(e->getText()));
                line.color = OUTPUT_COLOR;
            }
            catch (std::runtime_error &error)
            {
                line.text = std::string(">>>").append(
                    error.what());
                line.color = ERROR_COLOR;
            }
            history.push_back(line);
        })
        ->setWidthStretchMargin(0)
        ->setHeightStretchMargin(0);
}

void DebugWindow::nextTurn()
{
    ListBox *lstOutput = getWidget<ListBox>("lstOutput");
    lstOutput->clearItems();

    for (Output line : history)
    {
        ListBoxItem item;
        item.setText(line.text);
        lstOutput->addItem(item);
    }
}