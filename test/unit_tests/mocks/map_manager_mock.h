#pragma once


#include "../../src/core/map_manager.h"
#include <gmock/gmock.h>

class MapManagerMock : public MapManager {
public:
    MOCK_METHOD1(getTile, Tile&(const Location& location));
    MOCK_METHOD1(isValidTile, bool(const Location& location));
    MOCK_METHOD0(getMapHeight, unsigned int());
    MOCK_METHOD0(getMapWidth, unsigned int());
    MOCK_METHOD0(getMapDepth, unsigned int());
};
