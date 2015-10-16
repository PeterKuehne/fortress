#include "consumable_system.h"
#include "consumable_component.h"
#include "health_component.h"

void ConsumableSystem::handleEvent (const Event* event)
{
    switch (event->getType()) {
        case EVENT_CONSUME_ITEM: {
            const ConsumeItemEvent* l_event = dynamic_cast<const ConsumeItemEvent*> (event);
            LOG(INFO) << "Handling Consume Event for " << l_event->entity << " with " << l_event->item << std::endl;
            ConsumableComponent* consumable = getEngine()->getComponents()->get<ConsumableComponent> (l_event->item);
            if (!consumable) {
                LOG(ERROR) << "Consume event on non-consumable item: " << l_event->item << " !" << std::endl;
                return;
            }

            HealthComponent* health = getEngine()->getComponents()->get<HealthComponent> (l_event->entity);
            if (health) {
                if (consumable->quenches == THIRST) {
                    health->thirst += consumable->quenchStrength;
                } else if (consumable->quenches == HUNGER) {
                    health->hunger += consumable->quenchStrength;
                }

                if (consumable->effect == HEALTH_EFFECT) {
                    health->health += consumable->effectStrength;
                }
            }
            getEngine()->getEntities()->destroyEntity (l_event->item);
            break;
        }
        default: break;
    }
}
