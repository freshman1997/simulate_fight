#include "game/hero.h"
#include "game/ai.h"
#include "game/equip.h"
#include "game/event/event.h"

HeroBase::HeroBase() : id(-1)
{
    init();
}

void HeroBase::init()
{
    for (auto &it : this->equipments) {
        it->on_begin();
    }
}

void HeroBase::update(float deltaTime)
{
    this->ai->update(deltaTime);
}

void HeroBase::move()
{

}

void HeroBase::attack()
{
    // TODO 增加蓝量、造成伤害

}

void HeroBase::on_being_attack()
{
    // TODO 增加蓝量、调用装备的接口、

}

void HeroBase::buff_action()
{
    
}


