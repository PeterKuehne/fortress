#include "equipment_system.h"

void EquipmentSystem::handleEvent (const Event* event) {
    switch (event->getType()) {
        case EVENT_DROP_EQUIPMENT: {
            const DropEquipmentEvent* l_event = dynamic_cast<const DropEquipmentEvent*> (event);
            EquipmentComponent* equipment = getEngine()->getEntities()->getEquipments()->get(l_event->entity);
            LocationComponent* location = getEngine()->getEntities()->getLocations()->get(l_event->entity);
            if (l_event->item == equipment->rightHandWieldable) {
                equipment->rightHandWieldable = 0;
            }
            if (l_event->item == equipment->leftHandWieldable) {
                equipment->leftHandWieldable = 0;
            }
            if (l_event->item == equipment->headWearable) {
                equipment->headWearable = 0;
            }
            if (l_event->item == equipment->faceWearable) {
                equipment->faceWearable = 0;
            }
            if (l_event->item == equipment->armsWearable) {
                equipment->armsWearable = 0;
            }
            if (l_event->item == equipment->chestWearable) {
                equipment->chestWearable = 0;
            }
            if (l_event->item == equipment->legsWearable) {
                equipment->legsWearable = 0;
            }
            if (l_event->item == equipment->feetWearable) {
                equipment->feetWearable = 0;
            }
            SpriteComponent sprite;
            sprite.fgColor = Color (RED);
            sprite.sprite = '*';
            getEngine()->getEntities()->getSprites()->add (l_event->item, sprite);

            LocationComponent loc;
            loc.x = location->x;
            loc.y = location->y;
            loc.z = location->z;
            getEngine()->getEntities()->getLocations()->add (l_event->item, loc);

            break;
        }
        case EVENT_PICKUP_EQUIPMENT: {
            const PickupEquipmentEvent* l_event = dynamic_cast<const PickupEquipmentEvent*> (event);
            EquipmentComponent* equipment = getEngine()->getEntities()->getEquipments()->get(l_event->entity);
            getEngine()->getEntities()->getSprites()->remove(l_event->item);
            getEngine()->getEntities()->getLocations()->remove(l_event->item);

            equipment->carriedEquipment.push_back (l_event->item);

            break;
        }
        case EVENT_EQUIP_ITEM: {
            break;
        }
        case EVENT_UNEQUIP_ITEM: {
            const UnequipItemEvent* l_event = dynamic_cast<const UnequipItemEvent*> (event);
            EquipmentComponent* equipment = getEngine()->getEntities()->getEquipments()->get(l_event->entity);
            WearableComponent* wearable = getEngine()->getEntities()->getWearables()->get(l_event->entity);
            WieldableComponent* wieldable = getEngine()->getEntities()->getWieldables()->get(l_event->entity);
            bool equipped = false;
            if (wieldable != 0) {
                if (wieldable->position == WieldableRightHand || wieldable->position == WieldableBothHands) {
                    equipment->rightHandWieldable = l_event->item; equipped = true;
                }
                if (wieldable->position == WieldableLeftHand || wieldable->position == WieldableBothHands) {
                    equipment->leftHandWieldable = l_event->item; equipped = true;
                }
            }
            if (wearable != 0) {
                if (wearable->position == WearableHead) {
                    equipment->headWearable = l_event->item; equipped = true;
                }
                if (wearable->position == WearableFace) {
                    equipment->faceWearable = l_event->item; equipped = true;
                }
                if (wearable->position == WearableArms) {
                    equipment->armsWearable = l_event->item; equipped = true;
                }
                if (wearable->position == WearableChest) {
                    equipment->chestWearable = l_event->item; equipped = true;
                }
                if (wearable->position == WearableLegs) {
                    equipment->legsWearable = l_event->item; equipped = true;
                }
                if (wearable->position == WearableFeet) {
                    equipment->feetWearable = l_event->item; equipped = true;
                }
            }
            if (equipped) {
                for (size_t ii = 0; ii < equipment->carriedEquipment.size(); ii++) {
                    if (equipment->carriedEquipment[ii] == l_event->item) {
                        //equipment->carriedEquipment.delete(ii);
                    }
                }
            }
            break;
        }
        default: break;
    }
}
