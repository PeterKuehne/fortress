#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class GameEngineInterface;

#include "entity.h"
#include "tile.h"
#include "utility.h"
#include <vector>

class Location;

class EntityManagerInterface {
public:
    virtual ~EntityManagerInterface() {}

    virtual void initialise(GameEngineInterface* engine) = 0;
    virtual void destroy() = 0;

    virtual EntityId createEntity(Location& location) = 0;
    virtual void addEntity(EntityId id, Location& location) = 0;
    virtual void destroyEntity(EntityId id) = 0;
    virtual EntityId getPlayer() = 0;
    virtual EntityId getMaxId() = 0;

    virtual Location getLocation(EntityId entity) = 0;
    virtual void setLocation(EntityId entity, Location& location) = 0;
    virtual EntityHolder& get(unsigned int area) = 0;
};

#endif
