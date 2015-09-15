#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "entity.h"
#include "entity_manager_interface.h"
#include "utility.h"
#include <map>
#include <vector>

typedef std::map<EntityId, Location> LocationMap;
typedef LocationMap::const_iterator LocationConstIter;
typedef LocationMap::iterator LocationIter;

class EntityManager : public EntityManagerInterface {
public:
    void initialise (GameEngineInterface* engine);
    void destroy() {}

    EntityId createEntity (Location& location);
    void destroyEntity (EntityId);
    EntityId getPlayer ();
    EntityId getMaxId() { return m_maxId; }

    EntityId createWallPrefab (Location& location);
    EntityId createPlayerPrefab (Location& location);
    EntityId createEnemyPrefab (Location& location);
    EntityId createTilePrefab (Location& location);
    EntityId createMarkerPrefab (Location& location);
    EntityId createStairPrefab (STAIR dir, Location& location);
    EntityId createWeaponPrefab ();
    EntityId createShieldPrefab ();
    EntityId createHelmetPrefab ();

    EntityHolder findEntitiesNear (unsigned int x, unsigned int y, unsigned radius);
    EntityHolder findEntitiesAt (unsigned int x, unsigned int y);
    EntityHolder findEntitiesToThe (DIRECTION a_direction, EntityId a_entity);

    Location getLocation (EntityId entity) { return m_locations[entity]; }
    void setLocation (EntityId entity, Location& location);

    EntityHolder& get(); 
private:
    GameEngineInterface*                m_engine;
    unsigned long                       m_maxId;
    EntityId                            m_player;
    LocationMap                         m_locations;
    std::map<unsigned int, EntityHolder> m_entities;
};

#endif
