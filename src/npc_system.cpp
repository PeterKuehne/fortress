#include "npc_system.h"
#include <cstdlib>

void NpcSystem::handleEvent (const Event* event)
{

}

void NpcSystem::update ()
{
    if (getEngine()->isPlayerTurn()) return;
    std::map<EntityId, SpriteComponent>& l_sprites = getEngine()->getEntities()->getSprites()->getAll();
    std::map<EntityId, SpriteComponent>::iterator iter = l_sprites.begin();

    for (; iter != l_sprites.end(); iter++) {
        EntityId l_entity = iter->first;
        NpcComponent* l_npc = getEngine()->getEntities()->getNpcs()->get (l_entity);
        Location l_loc = getEngine()->getEntities()->getLocation(l_entity);
        if (l_npc == 0) continue;
        if (l_loc.z != getEngine()->getLevel()) continue;

        DIRECTION dir = Direction::None;
        // Check if player is attackable
        dir = getPlayerDirectionIfAttackable (iter->first);
        if (dir != Direction::None) {
            AttackEntityEvent* l_event = new AttackEntityEvent;
            l_event->entity = iter->first;
            l_event->direction = dir;
            getEngine()->raiseEvent (l_event);
        }

        // Check if player is nearby
        dir = getPlayerDirectionIfNearby (iter->first);
        if (dir == Direction::None) {
            dir = getRandomDirection();
        }
        MoveEntityEvent* l_event = new MoveEntityEvent();
        l_event->entity = iter->first;
        l_event->direction = dir;
        getEngine()->raiseEvent (l_event);
    }
    getEngine()->swapTurn();
}

DIRECTION NpcSystem::getRandomDirection () {
    return rand () % Direction::NorthEast;
}

DIRECTION NpcSystem::getPlayerDirectionIfNearby (EntityId enemy)
{
    EntityId player = getEngine()->getEntities()->getPlayer();
    Location playerLoc = getEngine()->getEntities()->getLocation(player);
    Location enemyLoc = getEngine()->getEntities()->getLocation(enemy);

    int xDiff = playerLoc.x - enemyLoc.x;
    int yDiff = playerLoc.y - enemyLoc.y;
    if ((xDiff > -5 && xDiff < 5) &&
        (yDiff > -5 && yDiff < 5) &&
        (enemyLoc.z == getEngine()->getLevel())) {
        if (abs(xDiff) > abs(yDiff)) {
            // Move horizontally first
            if (xDiff > 0) return Direction::East;
            return Direction::West;
        } else {
            // Move vertically first
            if (xDiff > 0) return Direction::South;
            return Direction::North;
        }
    }

    return Direction::None;
}

DIRECTION NpcSystem::getPlayerDirectionIfAttackable (EntityId entity) {

    EntityHolder l_entities;
    EntityId player = getEngine()->getEntities()->getPlayer();

    l_entities = getEngine()->getEntities()->findEntitiesToThe (Direction::North, entity);
    for (EntityId entity : l_entities) {
        if (entity == player) return Direction::North;
    }

    l_entities = getEngine()->getEntities()->findEntitiesToThe (Direction::East, entity);
    for (EntityId entity : l_entities) {
            if (entity == player) return Direction::East;
    }

    l_entities = getEngine()->getEntities()->findEntitiesToThe (Direction::South, entity);
    for (EntityId entity : l_entities) {
            if (entity == player) return Direction::South;
    }

    l_entities = getEngine()->getEntities()->findEntitiesToThe (Direction::West, entity);
    for (EntityId entity : l_entities) {
            if (entity == player) return Direction::West;
    }

    return Direction::None;
}
