#pragma once

#include "../core/entity.h"
#include "../core/location.h"
#include "../core/utility.h"
#include "component_base.h"
#include <map>
#include <vector>

struct NpcComponent : ComponentBase {
    std::vector<Location> path;
    std::string state = "";
    EntityId target = 0;
    std::map<std::string, std::string> attribs;
    std::string stateMachine = "";
    unsigned int losDistance = 15;
};
