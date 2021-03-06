#ifndef CONSUMABLE_COMPONENT_H
#define CONSUMABLE_COMPONENT_H

#include "component_base.h"
typedef enum {
    HUNGER = 0,
    THIRST = 1,
} HUNGER_THIRST;

typedef enum {
    HEALTH_EFFECT = 1,
} EFFECT;

struct ConsumableComponent : ComponentBase {
    HUNGER_THIRST quenches;
    int quenchStrength;
    EFFECT effect;
    int effectStrength;
};

#endif
