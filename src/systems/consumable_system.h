#include "../core/game_system_base.h"
#include "../core/utility.h"

class ConsumableSystem : public GameSystemBase {
private:
    void handleConsumeItemEvent(const ConsumeItemEvent* event) override;
};
