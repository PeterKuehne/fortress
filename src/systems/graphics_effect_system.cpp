#include <graphics_effect_system.h>
#include <glog/logging.h>

void GraphicsEffectSystem::update ()
{
    for (EntityId l_entity : getEngine()->getEntities()->get()) {
    	GraphicsEffectComponent* effect = getEngine()->getComponents()->get<GraphicsEffectComponent>(l_entity);
    	if (!effect) continue;

    	SpriteComponent* sprite = getEngine()->getComponents()->get<SpriteComponent>(l_entity);
    	if (!sprite) {
    		LOG(WARNING) << "GraphicsEffect without a sprite is pointless: " << l_entity << std::endl;
    		continue;
    	}

    	if (effect->ticks == 0) {
    		effect->org_color = sprite->fgColor;
    		effect->org_tile = sprite->sprite;
    		std::cout << getEngine()->getTick() << ": Setting org values" << std::endl;
    	}
    	if (effect->duration && effect->ticks > effect->duration) {
    		sprite->fgColor = effect->org_color;
    		sprite->sprite = effect->org_tile;
    		getEngine()->getComponents()->remove<GraphicsEffectComponent>(l_entity);
    		return;
    	}

    	switch (effect->type) {
    	case EFFECT_BLINK:
    			blinkEffect (effect, sprite);
    		break;
    	default:
    		break;
    	}

    	effect->ticks += 1;
    }
}

void GraphicsEffectSystem::blinkEffect (GraphicsEffectComponent* effect, SpriteComponent* sprite)
{
	if (effect->ticks % 10 == 0) {
		if (sprite->sprite != ' ') {
			sprite->sprite = ' ';
		} else {
			sprite->sprite = effect->org_tile;
		}
	}
}