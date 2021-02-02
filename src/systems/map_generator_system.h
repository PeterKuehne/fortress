#pragma once

#include "../core/game_system_base.h"

class GenerateRuralMapEvent;

class MapGeneratorSystem : public GameSystemBase {
public:
    void registerHandlers() override;

private:
    void generate(const GenerateRuralMapEvent& event);
};