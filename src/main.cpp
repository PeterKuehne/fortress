#include "core/game_engine.h"
#include "core/graphics.h"
#include "systems/combat_system.h"
#include "systems/consumable_system.h"
#include "systems/dialog_system.h"
#include "systems/equipment_system.h"
#include "systems/experience_system.h"
#include "systems/fov_system.h"
#include "systems/graphics_effect_system.h"
#include "systems/health_system.h"
#include "systems/interaction_system.h"
#include "systems/logmessage_system.h"
#include "systems/map_generator_system.h"
#include "systems/movement_system.h"
#include "systems/npc_system.h"
#include "systems/objectives_system.h"
#include "systems/prefab_system.h"
#include <cstdlib>
#include <glog/logging.h>
#include <iostream>
#include <signal.h>

int main(int argc, char** argv) {
    FLAGS_log_dir = ".";
    google::InitGoogleLogging(argv[0]);

    try {
        std::shared_ptr<GraphicsInterface> l_graphics =
            std::make_shared<Graphics>();
        l_graphics->initialise(argc, argv);
        GameEngine* l_engine = new GameEngine(l_graphics);

        l_engine->addSystem(new MovementSystem());
        l_engine->addSystem(new CombatSystem());
        l_engine->addSystem(new NpcSystem());
        l_engine->addSystem(new FovSystem());
        // l_engine->addSystem(new ObjectivesSystem());
        l_engine->addSystem(new EquipmentSystem());
        l_engine->addSystem(new ConsumableSystem());
        l_engine->addSystem(new GraphicsEffectSystem());
        l_engine->addSystem(new HealthSystem());
        l_engine->addSystem(new InteractionSystem());
        l_engine->addSystem(new DialogSystem());
        l_engine->addSystem(new PrefabSystem());
        l_engine->addSystem(new ExperienceSystem());
        l_engine->addSystem(new LogMessageSystem());
        l_engine->addSystem(new MapGeneratorSystem());
        l_engine->initialise();
        l_graphics->spin();
    } catch (char const* err) {
        std::cerr << "Main: Exception caught: " << err << std::endl;
    } catch (const std::string& err) {
        std::cerr << "Main: Exception caught: " << err << std::endl;
    }
    return 0;
}
