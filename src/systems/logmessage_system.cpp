
#include "logmessage_system.h"
#include "../components/logmessage_component.h"

void LogMessageSystem::registerHandlers() {
    events()->subscribe<AddLogMessageEvent>(
        [=](std::shared_ptr<AddLogMessageEvent> event) {
            EntityId player = state()->player();
            auto messages =
                state()->components()->get<LogMessageComponent>(player);
            if (!messages) {
                messages =
                    state()->components()->make<LogMessageComponent>(player);
            }
            messages->messages.push_back(
                LogMessageComponent::Message(event->message, event->category));
            // std::cout << event->category << ": " << event->message <<
            // std::endl;
        });

    events()->subscribe<PrefabCreatedEvent>(
        [=](std::shared_ptr<PrefabCreatedEvent> event) {
            // Todo: This should go into the narrator system...
            if (event->prefab == "player") {
                events()->raise(std::make_shared<AddLogMessageEvent>(
                    "You find yourself in a forest."));
                events()->raise(std::make_shared<AddLogMessageEvent>(
                    "Stairs will lead you into the dungeons."));
                events()->raise(std::make_shared<AddLogMessageEvent>(
                    "Beware the troll, living in the depths", "warning"));
            }
        });
}