#include "dungeon_generator.h"
#include "game_engine.h"
#include "algorithm.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "sprite_component.h"
#include "stair_component.h"

static unsigned int getDistance (unsigned int start, unsigned int end, void* customData);
static unsigned int getPathCost (unsigned int index, void* customData);
static unsigned int findNeighbours4 (unsigned int index, unsigned int* neighbours, void* customData);
static unsigned int findNeighbours8 (unsigned int index, unsigned int* neighbours, void* customData);

const char WALL     = '#';
const char CORNER   = 'C';
const char FLOOR    = '.';
const char DOOR     = 'D';
const char EMPTY    = ' ';
const char RESTRICTED = 'X';
const char ORC      = 'O';
const char UP       = '<';
const char DOWN     = '>';

void DungeonGenerator::reset () {
    GeneratorInterface::reset();

    m_startRoom = 0;
    m_rooms.clear();
}

bool DungeonGenerator::generate () {
    reset();
    initMap (EMPTY);

    for (unsigned r = 0; r < m_roomTarget; r++) {
        bool success = false;
        int x = 0;
        do {
            success = generateRoom ();
        } while (!success && x++ < 100);
        if (x >= 100) {
            std::cout << "Overran 100 tried to create room: " << r << std::endl;
            return false;
        }
    }
    for (size_t ii = 0; ii < m_rooms.size()-1; ii++) {
        connectRooms (m_rooms[ii], m_rooms[ii+1]);
    }

    placeUpStair();
    placeDownStair();
    placeOrcs();
    ///loadMap ();
    createEntitiesFromMap();
    reset();

    return true;
}

void DungeonGenerator::createEntitiesFromMap () {
    EntityId l_entity = 0;

    for (unsigned int yy = 0; yy < m_mapHeight; yy++) {
        for (unsigned int xx = 0; xx < m_mapWidth; xx++) {
            Location location;
            location.x = xx;
            location.y = yy;
            location.z = m_level;
            switch (getByCoordinate(xx, yy)) {
                case WALL:
                case CORNER:
                    l_entity = m_engine->getEntities()->createWallPrefab (location);
                    m_engine->getComponents()->get<SpriteComponent>(l_entity)->sprite = wallSprite (xx, yy);
                    break;
                case UP:
                    l_entity = m_engine->getEntities()->createStairPrefab (STAIR_UP, location);
                    if (m_level == 1) {
                        m_engine->getComponents()->get<StairComponent>(l_entity)->target = m_upStairTarget;
                        m_upStairLink = l_entity;
                    }
                    break;
                case DOWN:
                    l_entity = m_engine->getEntities()->createStairPrefab (STAIR_DOWN, location);
                    if (m_level == 1) m_engine->getComponents()->get<StairComponent>(l_entity)->target = m_downStairTarget;
                    break;
                case ORC:
                    m_engine->getEntities()->createEnemyPrefab (location);
                    m_engine->getEntities()->createTilePrefab (location);
                    break;
                case FLOOR:
                    m_engine->getEntities()->createTilePrefab (location);
                    break;
                case RESTRICTED:
                    break;
                default:
                    l_entity = m_engine->getEntities()->createMarkerPrefab (location);
                    m_engine->getComponents()->get<SpriteComponent>(l_entity)->sprite = getByCoordinate (xx, yy);
                    break;
            }
        }
    }
}

bool DungeonGenerator::generateRoom () {
    unsigned int width  = Utility::randBetween (5, 13);
    unsigned int height = Utility::randBetween (5, 13);
    unsigned int left   = Utility::randBetween (2, (m_mapWidth - width - 2));
    unsigned int top    = Utility::randBetween (2, (m_mapHeight - height - 2));

    if (left+width >= m_mapWidth || left < 1 ||
        top+height >= m_mapHeight || top < 1) {
            return false;
    }

    for (unsigned int yy = top-2; yy <= top+height+1; yy++) {
        for (unsigned int xx = left-2; xx <= left+width+1; xx++) {
            if (getByCoordinate (xx, yy) != EMPTY) return false;
        }
    }

    for (unsigned int yy = top-2; yy <= top+height+1; yy++) {
        for (unsigned int xx = left-2; xx <= left+width+1; xx++) {
            if (yy < top || yy >= top+height || xx < left || xx >= left+width) {
                getByCoordinate(xx, yy) = RESTRICTED;
            } else if (yy == top || yy == top+height-1 || xx == left || xx == left+width-1) {
                if ((yy <= top+1 || yy >= top+height-2) && (xx <= left+1 || xx >= left+width-2)) {
                    getByCoordinate(xx, yy) = CORNER;
                } else {
                    getByCoordinate(xx, yy) = WALL;
                }
            } else {
                getByCoordinate(xx, yy) = FLOOR;
            }
        }
    }

    Room l_room;
    l_room.x = left;
    l_room.y = top;
    l_room.width = width;
    l_room.height = height;
    l_room.midX = left + (width/2);
    l_room.midY = top + (height/2);
    l_room.index = CoordToIndex (l_room.midX, l_room.midY);
    m_rooms.push_back (l_room);

    return true;
}

void DungeonGenerator::connectRooms (Room& start, Room& end)
{
    Algorithm algo;
    algo.setCustomData (this);
    algo.setCostFunction (getPathCost);
    algo.setDistanceFunction (getDistance);
    algo.setNeighbourFunction (findNeighbours4);
    algo.setNumNeighbours (4);

    PathVector l_path;
    algo.findPath (start.index, end.index, l_path);
    //std::cout << "Found a " << l_path.size() << " tile corridor" << std::endl;

    for (size_t ii = 0; ii < l_path.size(); ii++) {
        unsigned char& tile = getByIndex(l_path[ii]);
        unsigned int x, y;
        IndexToCoord (l_path[ii], x, y);

        // Create tiles and doors
        if (tile == WALL || tile == DOOR) {
            tile = DOOR;
        } else {
            tile = FLOOR;
        }
    }

    for (size_t ii = 0; ii < l_path.size(); ii++) {
        // Wall of neighbours if necessary
        unsigned int neighbours[8] = {0};
        size_t count = findNeighbours8 (l_path[ii], neighbours, this);
        for (size_t ii = 0; ii < count; ii++) {
            unsigned char& adj = getByIndex (neighbours[ii]);
            if (adj == EMPTY || adj == RESTRICTED) {
                adj = WALL;
            }
        }
    }
}

void DungeonGenerator::placeUpStair()
{
    m_startRoom = Utility::randBetween (0, m_rooms.size()-1);
    getByCoordinate (m_rooms[m_startRoom].midX, m_rooms[m_startRoom].midY) = UP;
}

void DungeonGenerator::placeDownStair()
{
    if (m_rooms.size() < 2) return; // No point
    unsigned int room = m_startRoom;
    while (room == m_startRoom) room = Utility::randBetween (0,m_rooms.size()-1);
    getByCoordinate (m_rooms[room].midX, m_rooms[room].midY) = DOWN;
}

void DungeonGenerator::placeOrcs()
{
    if (m_rooms.size() < 2) return; // No point

    unsigned int numOrcs = Utility::randBetween (1, m_rooms.size() + m_level);
    for (size_t ii = 0; ii < numOrcs; ii++) {
        unsigned int room = 0;
        while (1) {
            room = Utility::randBetween (0, m_rooms.size()-1);
            if (m_startRoom == room) continue;
            unsigned int x, y;
            x = m_rooms[room].x + (Utility::randBetween (0, m_rooms[room].width-2)) + 1;
            y = m_rooms[room].y + (Utility::randBetween (0, m_rooms[room].height-2)) + 1;
            unsigned char& tile = getByCoordinate (x, y);
            if (tile == FLOOR) {
                tile = ORC;
                break;
            }
        }
    }
}

void DungeonGenerator::loadMap ()
{
    /*
    std::ifstream file ("../maps/test.map");
    char line[m_mapWidth];
    int lineCnt = 0;
    do {
        memset (line, EMPTY, sizeof (line));
        file.getline (line, sizeof (line));
        memcpy (m_map+(m_mapWidth*lineCnt), line, sizeof(line));
        lineCnt++;
    } while (file.gcount() > 0);
    */
}

unsigned char DungeonGenerator::wallSprite (unsigned int x, unsigned int y)
{
    unsigned char left   = isValidCoordinate (x-1, y) ? getByCoordinate (x-1, y) : 0;
    unsigned char up     = isValidCoordinate (x, y-1) ? getByCoordinate (x, y-1) : 0;
    unsigned char right  = isValidCoordinate (x+1, y) ? getByCoordinate (x+1, y) : 0;
    unsigned char down   = isValidCoordinate (x, y+1) ? getByCoordinate (x, y+1) : 0;

    int sprite_key = 0;
    if (left == WALL || left == CORNER)     sprite_key |= 8;
    if (up == WALL || up == CORNER)         sprite_key |= 4;
    if (right == WALL || right == CORNER)   sprite_key |= 2;
    if (down == WALL || down == CORNER)     sprite_key |= 1;

    switch (sprite_key) {
        case  0: return 206; break;
        case  1: return 210; break;
        case  2: return 198; break;
        case  3: return 201; break;
        case  4: return 208; break;
        case  5: return 186; break;
        case  6: return 200; break;
        case  7: return 204; break;
        case  8: return 181; break;
        case  9: return 187; break;
        case 10: return 205; break;
        case 11: return 203; break;
        case 12: return 188; break;
        case 13: return 185; break;
        case 14: return 202; break;
        case 15: return 206; break;
    }
    std::cout << "Return default wall" << std::endl;
    return 206;
}

unsigned int getPathCost (unsigned int index, void* customData)
{
    DungeonGenerator* l_gen = static_cast<DungeonGenerator*> (customData);
    unsigned char point = l_gen->getByIndex(index);
    unsigned int cost = 0;
    switch (point) {
        case EMPTY:
            cost = 3;
            break;
        case WALL:
        case RESTRICTED:
        case DOOR:
            cost = 5;
            break;
        case FLOOR:
            cost = 0;
            break;
        case CORNER:
            cost = 999;
            break;
        default:
            cost = 999;
            break;
    }
    //std::cout << "Returning cost: " << cost << std::endl;
    return cost;
}

unsigned int findNeighbours4 (unsigned int index, unsigned int* neighbours, void* customData)
{
    DungeonGenerator* l_gen = static_cast<DungeonGenerator*> (customData);
    unsigned int indexX, indexY;
    l_gen->IndexToCoord (index, indexX, indexY);

    unsigned int count = 0;

    if (l_gen->isValidCoordinate (indexX-1, indexY))
        neighbours[count++] = l_gen->CoordToIndex (indexX-1, indexY);
    if (l_gen->isValidCoordinate (indexX+1, indexY))
        neighbours[count++] = l_gen->CoordToIndex (indexX+1, indexY);
    if (l_gen->isValidCoordinate (indexX, indexY-1))
        neighbours[count++] = l_gen->CoordToIndex (indexX, indexY-1);
    if (l_gen->isValidCoordinate (indexX, indexY+1))
        neighbours[count++] = l_gen->CoordToIndex (indexX, indexY+1);
    //std::cout << "Return " << count << " neightbours" << std::endl;

    return count;
}

unsigned int findNeighbours8 (unsigned int index, unsigned int* neighbours, void* customData)
{
    DungeonGenerator* l_gen = static_cast<DungeonGenerator*> (customData);
    unsigned int indexX, indexY;
    l_gen->IndexToCoord (index, indexX, indexY);

    unsigned int count = 0;

    if (l_gen->isValidCoordinate (indexX-1, indexY))
        neighbours[count++] = l_gen->CoordToIndex (indexX-1, indexY);
    if (l_gen->isValidCoordinate (indexX+1, indexY))
        neighbours[count++] = l_gen->CoordToIndex (indexX+1, indexY);
    if (l_gen->isValidCoordinate (indexX, indexY-1))
        neighbours[count++] = l_gen->CoordToIndex (indexX, indexY-1);
    if (l_gen->isValidCoordinate (indexX, indexY+1))
        neighbours[count++] = l_gen->CoordToIndex (indexX, indexY+1);
    if (l_gen->isValidCoordinate (indexX-1, indexY-1))
        neighbours[count++] = l_gen->CoordToIndex (indexX-1, indexY-1);
    if (l_gen->isValidCoordinate (indexX+1, indexY-1))
        neighbours[count++] = l_gen->CoordToIndex (indexX+1, indexY-1);
    if (l_gen->isValidCoordinate (indexX-1, indexY+1))
        neighbours[count++] = l_gen->CoordToIndex (indexX-1, indexY+1);
    if (l_gen->isValidCoordinate (indexX+1, indexY+1))
        neighbours[count++] = l_gen->CoordToIndex (indexX+1, indexY+1);

    //std::cout << "Return " << count << " neightbours" << std::endl;

    return count;
}

unsigned int getDistance (unsigned int start, unsigned int end, void* customData)
{
    DungeonGenerator* l_gen = static_cast<DungeonGenerator*> (customData);
    unsigned int startX, startY;
    unsigned int endX, endY;
    unsigned int distance = 0;
    l_gen->IndexToCoord (start, startX, startY);
    l_gen->IndexToCoord (end, endX, endY);
    distance = (abs (startX-endX)+abs(startY-endY));
    //std::cout << "Returning distance: " << distance << std::endl;
    return distance;
}
