#include "game/hero.h"
#include "game/ai.h"

void HeroBase::update()
{
    if (this->ai) {
        this->ai->update();
    }
}
