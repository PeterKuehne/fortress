#ifndef __GRAPHICS_MOCK_H__
#define __GRAPHICS_MOCK_H__

#include <gmock/gmock.h>
#include "../../src/core/graphics.h"

class GraphicsMock : public Graphics {
public:
    MOCK_METHOD5 (drawTile, void (int y, int x, unsigned int tile, Color fg, Color bg));
    MOCK_METHOD5 (drawString, unsigned int (int y, int x, const char* s, Color fg, Color bg));
};

#endif
