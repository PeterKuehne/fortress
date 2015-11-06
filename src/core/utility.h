#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <climits>
#include <glog/logging.h>

typedef unsigned short DIRECTION;
namespace Direction {
    const DIRECTION None        = 0;
    const DIRECTION North       = 1;
    const DIRECTION East        = 2;
    const DIRECTION South       = 3;
    const DIRECTION West        = 4;
    const DIRECTION NorthEast   = 5;
    const DIRECTION SouthEast   = 6;
    const DIRECTION SouthWest   = 7;
    const DIRECTION NorthWest   = 8;

}

enum {
    KEY_TAB 	= 1,
    KEY_ESC 	= 2,
    KEY_ENTER	= 3,
    KEY_UP		= 4,
    KEY_DOWN	= 5,
    KEY_RIGHT	= 6,
    KEY_LEFT	= 7,
    KEY_MAX = 63
};


typedef enum {
    STAIR_UP    = 0,
    STAIR_DOWN  = 1
} STAIR;

struct Location {
    unsigned int x      = UINT_MAX;
    unsigned int y      = UINT_MAX;
    unsigned int z      = UINT_MAX;
    unsigned int area   = 0;
    bool operator== (const Location& rhs) { return (rhs.area == area && rhs.x == x && rhs.y == y && rhs.z == z); }
};
    std::ostream& operator<< (std::ostream& out, const Location& loc);

namespace Utility {
    unsigned int randBetween (unsigned int start, unsigned int end);
    bool randChance (unsigned int percentage);
};

#endif
