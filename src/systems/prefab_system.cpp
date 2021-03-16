#include "prefab_system.h"
#include "../components/debug_component.h"
#include "../components/experience_component.h"
#include "../components/key_component.h"
#include "../components/openable_component.h"
#include "../components/player_component.h"
#include "../core/yaml_converter.h"
#include <experimental/filesystem>
#include <glog/logging.h>
#include <iostream>
#include <yaml-cpp/yaml.h>

void PrefabSystem::registerHandlers() {
    events()->subscribe<InstantiatePrefabEvent>([=](auto event) {
        auto iter = m_prefabs.find(event.prefab);
        if (iter == m_prefabs.end()) {
            LOG(WARNING) << "Invalid prefab '" << event.prefab << "' requested"
                         << std::endl;
            return;
        }

        YAML::Node node = iter->second;
        EntityId entity = event.entity;

        addDescriptionComponent(node, entity);
        addSpriteComponent(node, entity);
        addColliderComponent(node, entity);
        addHealthComponent(node, entity);
        addDroppableComponent(node, entity);
        addConsumableComponent(node, entity);
        addOpenableComponent(node, entity);
        addWearableComponent(node, entity);
        addWieldableComponent(node, entity);
        addEquipmentComponent(node, entity);
        addConnectorComponent(node, entity);
        addNpcComponent(node, entity);
        addPlayerComponent(node, entity);
        addExperienceComponent(node, entity);
        addGroupingComponent(node, entity);
        addKeyComponent(node, entity);

        events()->fire<PrefabCreatedEvent>(event.entity, event.prefab);
    });

    std::string path = "../data/prefabs/";
    loadPrefabsFromDirectory(path);
}

void PrefabSystem::loadPrefabsFromDirectory(const std::string& directory) {
    namespace fs = std::experimental::filesystem;
    for (auto& filename : fs::directory_iterator(directory)) {
        YAML::Node node = YAML::LoadFile(filename.path().relative_path().generic_string());
        addPrefab(filename.path().stem().generic_string(), node);
    }
}

void PrefabSystem::addPrefab(const std::string& name, const YAML::Node& node) {
    m_prefabs[name] = node;
}

void PrefabSystem::addDescriptionComponent(YAML::Node& node,
                                           EntityId entity) const {
    auto l_description = components()->make<DescriptionComponent>(entity);
    l_description->title = node["name"].as<std::string>("Unknown");
    l_description->text =
        node["description"].as<std::string>("It's hard to describe");
}

void PrefabSystem::addSpriteComponent(YAML::Node& node, EntityId entity) const {
    if (!node["symbol"].IsDefined()) {
        return;
    }

    auto l_sprite = components()->make<SpriteComponent>(entity);
    l_sprite->fgColor = node["foreground-color"].as<Color>(Color(WHITE));
    l_sprite->bgColor = node["background-color"].as<Color>(Color(BLACK));
    unsigned int sprite = node["symbol"].as<unsigned int>(0);
    l_sprite->sprite = sprite ? sprite : node["symbol"].as<char>('?');
}

void PrefabSystem::addColliderComponent(YAML::Node& node,
                                        EntityId entity) const {
    if (!node["collidable"].as<bool>(true)) {
        return;
    }
    components()->make<ColliderComponent>(entity);
}

void PrefabSystem::addHealthComponent(YAML::Node& node, EntityId entity) const {
    if (!node["health"].IsDefined()) {
        return;
    }
    auto l_health = components()->make<HealthComponent>(entity);
    l_health->health = node["health"].as<unsigned int>(1);
}

void PrefabSystem::addDroppableComponent(YAML::Node& node,
                                         EntityId entity) const {
    if (!node["droppable"].IsDefined()) {
        return;
    }
    components()->make<DroppableComponent>(entity);
}
void PrefabSystem::addConsumableComponent(YAML::Node& node,
                                          EntityId entity) const {
    if (!node["consumable"].IsDefined()) {
        return;
    }
    auto l_consumable = components()->make<ConsumableComponent>(entity);
    l_consumable->quenches =
        static_cast<HUNGER_THIRST>(node["consumable"]["quenches"].as<int>(0));
    l_consumable->quenchStrength = node["consumable"]["strength"].as<int>(0);
    l_consumable->effect =
        static_cast<EFFECT>(node["consumable"]["effect"].as<int>(0));
    l_consumable->effectStrength =
        node["consumable"]["effectStrength"].as<int>(0);
}

void PrefabSystem::addOpenableComponent(YAML::Node& node,
                                        EntityId entity) const {
    if (!node["openable"].IsDefined()) {
        return;
    }
    auto l_openable = components()->make<OpenableComponent>(entity);
    l_openable->open = node["openable"]["open"].as<bool>(false);
}

void PrefabSystem::addWearableComponent(YAML::Node& node,
                                        EntityId entity) const {
    if (!node["wearable"].IsDefined()) {
        return;
    }
    auto l_wearable = components()->make<WearableComponent>(entity);
    l_wearable->position =
        static_cast<WearablePosition>(node["wearable"]["position"].as<int>(0));
}
void PrefabSystem::addWieldableComponent(YAML::Node& node,
                                         EntityId entity) const {
    if (!node["wieldable"].IsDefined()) {
        return;
    }
    auto l_wieldable = components()->make<WieldableComponent>(entity);
    l_wieldable->position = static_cast<WieldablePosition>(
        node["wieldable"]["position"].as<int>(0));
    l_wieldable->baseDamage = node["wieldable"]["damage"].as<int>(0);
    l_wieldable->baseDefence = node["wieldable"]["defence"].as<int>(0);
}

void PrefabSystem::addEquipmentComponent(YAML::Node& node,
                                         EntityId entity) const {
    if (!node["equipment"].IsDefined()) {
        return;
    }
    Location invalidLoc;

    auto l_equipment = components()->make<EquipmentComponent>(entity);
    l_equipment->maxCarryVolume = node["equipment"]["maxVolume"].as<int>(0);
    l_equipment->maxCarryWeight = node["equipment"]["maxWeight"].as<int>(0);
    l_equipment->headWearable = instantiatePrefab(
        node["equipment"]["head"].as<std::string>(""), invalidLoc);
    l_equipment->faceWearable = instantiatePrefab(
        node["equipment"]["face"].as<std::string>(""), invalidLoc);
    l_equipment->chestWearable = instantiatePrefab(
        node["equipment"]["chest"].as<std::string>(""), invalidLoc);
    l_equipment->armsWearable = instantiatePrefab(
        node["equipment"]["arms"].as<std::string>(""), invalidLoc);
    l_equipment->handsWearable = instantiatePrefab(
        node["equipment"]["hands"].as<std::string>(""), invalidLoc);
    l_equipment->legsWearable = instantiatePrefab(
        node["equipment"]["legs"].as<std::string>(""), invalidLoc);
    l_equipment->feetWearable = instantiatePrefab(
        node["equipment"]["feet"].as<std::string>(""), invalidLoc);

    l_equipment->rightHandWieldable = instantiatePrefab(
        node["equipment"]["rightHand"].as<std::string>(""), invalidLoc);
    l_equipment->leftHandWieldable = instantiatePrefab(
        node["equipment"]["leftHand"].as<std::string>(""), invalidLoc);

    if (node["equipment"]["carried"].IsSequence()) {
        for (auto& item :
             node["equipment"]["carried"].as<std::vector<std::string>>()) {
            l_equipment->carriedEquipment.push_back(
                instantiatePrefab(item, invalidLoc));
        }
    }
}

void PrefabSystem::addConnectorComponent(YAML::Node& node,
                                         EntityId entity) const {
    if (!node["connector"].IsDefined()) {
        return;
    }
    components()->make<ConnectorComponent>(entity);
}

void PrefabSystem::addNpcComponent(YAML::Node& node, EntityId entity) const {
    if (!node["smart"].IsDefined()) {
        return;
    }
    auto l_npc = components()->make<NpcComponent>(entity);
    l_npc->stateMachine = node["smart"]["fsm"].as<std::string>("human");
    l_npc->attribs["seek_target"] = node["smart"]["target"].as<std::string>("");
}

void PrefabSystem::addPlayerComponent(YAML::Node& node, EntityId entity) const {
    if (!node["player"].IsDefined()) {
        return;
    }
    components()->make<PlayerComponent>(entity);
    components()->make<DebugComponent>(entity);
}

void PrefabSystem::addExperienceComponent(YAML::Node& node,
                                          EntityId entity) const {
    if (!node["experience"].IsDefined()) {
        return;
    }
    components()->make<ExperienceComponent>(entity);
}

void PrefabSystem::addGroupingComponent(YAML::Node& node,
                                        EntityId entity) const {
    if (!node["groupings"].IsDefined()) {
        return;
    }
    auto l_grouping = components()->make<GroupingComponent>(entity);
    for (YAML::const_iterator iter = node["groupings"].begin();
         iter != node["groupings"].end(); ++iter) {
        std::string group(iter->as<std::string>());

        //        m_world->getGroupings().addEntityToGrouping(entity, group);
        l_grouping->groupings.push_back(group);
    }
}

void PrefabSystem::addKeyComponent(YAML::Node& node, EntityId entity) const {
    if (!node["key"].IsDefined()) {
        return;
    }
    components()->make<KeyComponent>(entity);
}
