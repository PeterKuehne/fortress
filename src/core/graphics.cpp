#include "graphics.h"
#include "utility.h"
#include <SOIL/SOIL.h>
#include <chrono>
#include <glog/logging.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <thread>

static Graphics* g_graphics = 0;

void resizeWindowCallBack(GLFWwindow*, int width, int height) {
    g_graphics->callResizeFunc(width, height);
}

void keyboardCallBack(GLFWwindow*, int key, int scancode, int action,
                      int mods) {
    g_graphics->callKeyboardFunc(key, scancode, action, mods);
}

void Graphics::callResizeFunc(int width, int height) {
    m_width = width;
    m_height = height;
    if (m_resizeFunc) {
        m_resizeFunc(width, height);
    }
}

void Graphics::callKeyboardFunc(int key, int scancode, int action, int mods) {
    // std::cout << "Key: " << key << " scancode: " << scancode << " action: "
    // << action << " mods " << mods << std::endl;
    if (key == 340)
        return; // Don't pass on shift key presses
    if (key == 49 && (mods & GLFW_MOD_SHIFT))
        key = '!'; // Shift 1
    if (key == 50 && (mods & GLFW_MOD_SHIFT))
        key = '"'; // Shift 2
    if (key == 51 && (mods & GLFW_MOD_SHIFT))
        key = '@'; // Shift 3
    if (key == 52 && (mods & GLFW_MOD_SHIFT))
        key = '$'; // Shift 4
    if (key == 53 && (mods & GLFW_MOD_SHIFT))
        key = '%'; // Shift 5
    if (key == 54 && (mods & GLFW_MOD_SHIFT))
        key = '^'; // Shift 6
    if (key == 55 && (mods & GLFW_MOD_SHIFT))
        key = '&'; // Shift 7
    if (key == 56 && (mods & GLFW_MOD_SHIFT))
        key = '*'; // Shift 8
    if (key == 57 && (mods & GLFW_MOD_SHIFT))
        key = '('; // Shift 9
    if (key == 48 && (mods & GLFW_MOD_SHIFT))
        key = ')'; // Shift 0
    if (key == 45 && (mods & GLFW_MOD_SHIFT))
        key = '_'; // Shift -
    if (key == 61 && (mods & GLFW_MOD_SHIFT))
        key = '+'; // Shift =
    if (key == 91 && (mods & GLFW_MOD_SHIFT))
        key = '{'; // Shift [
    if (key == 93 && (mods & GLFW_MOD_SHIFT))
        key = '}'; // Shift ]
    if (key > 64 && key < 91 && !(mods & GLFW_MOD_SHIFT))
        key += 32;
    if (key == GLFW_KEY_ESCAPE)
        key = KEY_ESC;
    if (key == GLFW_KEY_TAB)
        key = KEY_TAB;
    if (key == GLFW_KEY_ENTER)
        key = KEY_ENTER;
    if (key == GLFW_KEY_UP)
        key = KEY_UP;
    if (key == GLFW_KEY_DOWN)
        key = KEY_DOWN;
    if (key == GLFW_KEY_LEFT)
        key = KEY_LEFT;
    if (key == GLFW_KEY_RIGHT)
        key = KEY_RIGHT;
    if (key == GLFW_KEY_BACKSPACE)
        key = KEY_BACKSPACE;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (m_keyDownFunc)
            m_keyDownFunc((unsigned char)key, 0, 0);
    } else if (action == GLFW_RELEASE) {
        if (m_keyUpFunc)
            m_keyUpFunc((unsigned char)key, 0, 0);
    }
}

unsigned int Graphics::drawString(int y, int x, const char* string, Color fg,
                                  Color bg) {
    int offset = 0;
    while (*string != '\0') {
        drawRect(m_fontTexture, x + offset++, y,
                 static_cast<unsigned int>(*string++), fg, bg);
    }
    return offset;
}

void Graphics::drawTile(int y, int x, unsigned int tile, Color fg, Color bg) {
    drawRect(m_tileTexture, x, y, tile, fg, bg);
}

void Graphics::drawRect(Texture& texture, int x, int y, unsigned int tile,
                        Color fg, Color bg) {
    unsigned int tileCol = 0;
    unsigned int tileRow = 0;
    tileCol = tile % texture.numCols;
    tileRow = (tile - (tileCol)) / texture.numCols;

    y = (m_height / m_tileHeight) - (y + 1);

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glColor3f(fg.Red() / 256.0, fg.Green() / 256.0, fg.Blue() / 256.0);
    glBegin(GL_QUADS);
    glTexCoord2f(texture.tileWidth * (tileCol + 1),
                 texture.tileHeight * (tileRow + 1));
    glVertex2f(m_tileWidth * (x + 1), m_tileHeight * (y + 0));
    glTexCoord2f(texture.tileWidth * (tileCol + 1),
                 texture.tileHeight * (tileRow + 0));
    glVertex2f(m_tileWidth * (x + 1), m_tileHeight * (y + 1));
    glTexCoord2f(texture.tileWidth * (tileCol + 0),
                 texture.tileHeight * (tileRow + 0));
    glVertex2f(m_tileWidth * (x + 0), m_tileHeight * (y + 1));
    glTexCoord2f(texture.tileWidth * (tileCol + 0),
                 texture.tileHeight * (tileRow + 1));
    glVertex2f(m_tileWidth * (x + 0), m_tileHeight * (y + 0));
    glEnd();
}

void Graphics::drawBorder(int y, int x, int height, int width) {
    // Draw Corners
    drawTile(y, x, 201, Color(WHITE), Color(WHITE));
    drawTile(y + 1 + height, x, 200, Color(WHITE), Color(WHITE));
    drawTile(y, x + 1 + width, 187, Color(WHITE), Color(WHITE));
    drawTile(y + 1 + height, x + 1 + width, 188, Color(WHITE), Color(WHITE));

    // Draw Horizontals
    for (int xx = x + 1; xx <= x + width; xx++) {
        drawTile(y, xx, 205, Color(WHITE), Color(WHITE));
        drawTile(y + height + 1, xx, 205, Color(WHITE), Color(WHITE));
    }

    // Draw Verticals
    for (int yy = y + 1; yy <= y + height; yy++) {
        drawTile(yy, x, 186, Color(WHITE), Color(WHITE));
        drawTile(yy, x + width + 1, 186, Color(WHITE), Color(WHITE));
    }
}

void Graphics::clearArea(int y, int x, int height, int width) {
    for (int xx = x; xx < x + width; xx++) {
        for (int yy = y; yy < y + height; yy++) {
            drawTile(yy, xx, 219, Color(BLACK), Color(BLACK));
        }
    }
}

int Graphics::getScreenHeight() { return (m_height / m_tileHeight); }

int Graphics::getScreenWidth() { return (m_width / m_tileWidth); }

void Graphics::calculateWindowOffsetsFromCentre(int height, int width, int& y,
                                                int& x) {
    int screenWidth = getScreenWidth();
    int screenHeight = getScreenHeight();

    x = (screenWidth / 2) - (width / 2);
    y = (screenHeight / 2) - (height / 2);
}

void Graphics::updateScreenSize(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void Graphics::updateTileSize(unsigned int width, unsigned int height) {
    if (width)
        m_tileWidth = width;
    if (height)
        m_tileHeight = height;
}

void Graphics::setKeyDownFunc(KeyboardFuncPtr func) { m_keyDownFunc = func; }

void Graphics::setKeyUpFunc(KeyboardFuncPtr func) { m_keyUpFunc = func; }

void Graphics::setDisplayFunc(DisplayFuncPtr func) { m_displayFunc = func; }

void Graphics::setMouseFunc(MouseFuncPtr func) {
    // Todo: Implement mouse support
}

void Graphics::setResizeFunc(ResizeFuncPtr func) { m_resizeFunc = func; }

void Graphics::spin() {
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(m_window)) {
        double currTime = glfwGetTime();
        if (currTime > lastTime + 0.03) {
            m_displayFunc();
            glfwSwapBuffers(m_window);
            lastTime = currTime;
        }
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "Thank you for playing FORTRESS" << std::endl;
}

void Graphics::beginScreenUpdate() { glClear(GL_COLOR_BUFFER_BIT); }

void Graphics::endScreenUpdate() { glFlush(); }

void Graphics::initialise(int argc, char** argv) {
    if (!glfwInit())
        return;
    g_graphics = this;

    m_width = 640;
    m_height = 480;

    if (m_config.fullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_width = mode->width;
        m_height = mode->height;
    } else {
        m_width = m_config.windowWidth;
        m_height = m_config.windowHeight;
    }
    m_window = glfwCreateWindow(m_width, m_height, "FORTRESS", NULL, NULL);

    if (!m_window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(m_window, resizeWindowCallBack);
    glfwSetKeyCallback(m_window, keyboardCallBack);

    updateScreenSize(m_width, m_height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setDisplayFunc([]() {});

    m_tileTexture = loadTexture(m_config.tileset, m_config.tileset_rows,
                                m_config.tileset_cols);

    m_fontTexture = loadTexture(m_config.fontset, m_config.fontset_rows,
                                m_config.fontset_cols);
}

Texture Graphics::loadTexture(std::string name, unsigned int cols,
                              unsigned int rows) {
    Texture texture;

    texture.numRows = rows ? rows : 16;
    texture.numCols = cols ? cols : 16;

    LOG(INFO) << "Loading texture: " << name << " " << cols << "x" << rows
              << std::endl;

    texture.name = std::string("graphics/");
    // std::string tileset ("graphics/");
    texture.name.append(name);
    texture.id = SOIL_load_OGL_texture(
        texture.name.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT);
    if (texture.id == 0) {
        LOG(ERROR) << "Failed to load texture: " << texture.name << std::endl;
        glfwTerminate();
    }
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,
                             &texture.textureWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT,
                             &texture.textureHeight);

    texture.tileWidth = 1.0 / texture.numCols;
    texture.tileHeight = 1.0 / texture.numRows;

    LOG(INFO) << "Created texture " << texture.id << " (" << texture.name << ")"
              << " width: " << texture.textureWidth
              << " height: " << texture.textureHeight << std::endl;
    LOG(INFO) << "Texture is " << texture.numCols << " tiles across @ "
              << texture.tileWidth << " and " << texture.numRows
              << " tiles high @ " << texture.tileHeight << std::endl;

    return texture;
}
