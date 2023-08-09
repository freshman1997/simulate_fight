#include "game/hero.h"
#include "game/ai.h"
#include "game/equip.h"

void HeroBase::update()
{
    if (this->ai) {
        this->ai->update();
    }
}

HeroBase::HeroBase()
{
    init();
}

void HeroBase::init()
{
    for (auto &it : this->equipments) {
        it->on_begin();
    }
}
