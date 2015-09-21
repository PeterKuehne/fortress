#include "generator_window.h"
#include "game_engine.h"
#include "dungeon_generator.h"
#include "map_window.h"
#include <sstream>

std::string GeneratorWindow::formatNumber (int number) {
    std::stringstream str;

    if (number < 10) str << " ";
    if (number < 100) str << " ";
    str << number;
    return str.str();
}

void GeneratorWindow::gainFocus () {
    setTitle (" Create New World ");
    m_levelWidth = 50;
    m_levelHeight = 50;
    m_levelDepth = 5;
    m_levelRooms = 10;
    m_worldSize = 129;
    m_selectedPosition = NONE;
    m_generator = new DungeonGenerator();
    m_generator->initialise (getEngine());
    m_generatingLevel = 0;
    m_generating = false;
    m_generated = false;
}

void GeneratorWindow::resize() {
    setDimensions (0, 0, getEngine()->getGraphics()->getScreenWidth(), getEngine()->getGraphics()->getScreenHeight());
}

void GeneratorWindow::redraw() {

    if (getArgs()) return;

    int middleX = getWidth()/2;
    int middleY = getHeight();
    int selY    = 0;

    drawString (middleY - 11, middleX - 10, "Level Parameters:");
    drawString (middleY - 10, middleX - 6, "Width: ");
    if (!m_generating && !m_generated) drawTile (middleY - 10, middleX - 6, 'W', Color (GREEN), Color (BLACK));
        drawString (middleY - 10, middleX + 2, formatNumber(m_levelWidth).c_str());
        if (m_selectedPosition == WIDTH) selY = middleY - 10;
    drawString (middleY - 9, middleX - 6, "Height: ");
    if (!m_generating && !m_generated) drawTile (middleY - 9, middleX - 6, 'H', Color (GREEN), Color (BLACK));
        drawString (middleY - 9, middleX + 2, formatNumber(m_levelHeight).c_str());
        if (m_selectedPosition == HEIGHT) selY = middleY - 9;

    drawString (middleY - 8, middleX - 6, "Rooms: ");
    if (!m_generating && !m_generated) drawTile (middleY - 8, middleX - 6, 'R', Color (GREEN), Color (BLACK));
        drawString (middleY - 8, middleX + 2, formatNumber(m_levelRooms).c_str());
        if (m_selectedPosition == ROOMS) selY = middleY - 8;

    drawString (middleY - 7, middleX - 6, "Depth: ");
    if (!m_generating && !m_generated) drawTile (middleY - 7, middleX - 6, 'D', Color (GREEN), Color (BLACK));
        drawString (middleY - 7, middleX + 2, formatNumber(m_levelDepth).c_str());
        if (m_selectedPosition == DEPTH) selY = middleY - 7;

    if (!m_generating &&m_selectedPosition != NONE) {
        drawTile (selY, middleX + 1, '<', Color(RED), Color(BLACK));
        drawTile (selY, middleX + 5, '>', Color(RED), Color(BLACK));
    }

    drawString (middleY - 5, middleX - 6, "Create: ");
    if (!m_generating && !m_generated) drawTile (middleY - 5, middleX - 6, 'C', Color (GREEN), Color (BLACK));

    if (m_generating) {
        //drawString (middleY - 5, middleX + 3, ":");
        drawProgress (middleX + 2, middleY - 5, m_generatingLevel - 1, m_levelDepth);
    } else if (m_generated) {
        drawString (middleY - 5, middleX + 2, "Done!");
        drawString (middleY - 4, middleX - 6, "Play");
        drawTile (middleY - 4, middleX - 6, 'P', Color(GREEN), Color(BLACK));
    }

    std::string explanation ("Use '+' and '-' to change values");
    int offset = getWidth()/2 - explanation.length()/2;
    drawString (getHeight()-2, offset, explanation.c_str());
}

void GeneratorWindow::update () {
    if (getArgs() && !m_generated) {
        startGenerating();
        for (unsigned int level = 1; level <= m_levelDepth; level++) {
            generateLevel();
        }
        startPlaying();
        return;
    }

    if (m_generating) {
        generateLevel();
    }
}

void GeneratorWindow::keyDown (unsigned char key) {
    if (key == ESC) {
        getEngine()->getWindows()->popWindow();
    }

    if (m_generating) return; // Don't allow updates during generating

    if (m_generated && (key == 'p' || key == 'P')) {
        startPlaying();
        return; // Don't allow other actions
    }
    if (key == 'W' || key == 'w') m_selectedPosition = WIDTH;
    if (key == 'H' || key == 'h') m_selectedPosition = HEIGHT;
    if (key == 'R' || key == 'r') m_selectedPosition = ROOMS;
    if (key == 'D' || key == 'd') m_selectedPosition = DEPTH;

    if (key == 'C' || key == 'c') {
        startGenerating();
    }

    int adjustment = 0;
    if (key == '+') adjustment = 1;
    if (key == '-') adjustment = -1;
    switch (m_selectedPosition) {
        case WIDTH: m_levelWidth    += adjustment; break;
        case HEIGHT: m_levelHeight  += adjustment; break;
        case ROOMS: m_levelRooms    += adjustment; break;
        case DEPTH: m_levelDepth    += adjustment; break;
        default:break;
    }
}

void GeneratorWindow::startGenerating () {
    m_generatingLevel = 1;
    m_generating = true;
    getEngine()->getMap()->resetMap (m_levelWidth, m_levelHeight, m_levelDepth);
    getEngine()->setLevel (1);
}

void GeneratorWindow::generateLevel () {
    if (m_generatingLevel <= m_levelDepth) {
        m_generator->mapHeight()    = m_levelHeight;
        m_generator->mapWidth()     = m_levelWidth;
        m_generator->numberOfRooms()= m_levelRooms;
        m_generator->currentLevel() = m_generatingLevel++;
        while (!m_generator->generate());
    } else {
        m_generating = false;
        m_generated = true;
    }
}

void GeneratorWindow::startPlaying() {
    MapWindow* l_win = new MapWindow();
    l_win->initialise (getEngine());
    getEngine()->getWindows()->replaceWindow (l_win);
}
