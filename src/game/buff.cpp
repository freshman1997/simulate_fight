#include "game/buff.h"
#include "cfg/buff_cfg.h"
#include "manager/object_manager.hpp"
#include "game/hero.h"

REGISTER_TYPE("buff_base", BuffBase)

Object * BuffBase::clone()
{
    return new BuffBase;
}

Object * BuffBase::clone_and_clean()
{
    return clone();
}

void BuffBase::update(float deltaTime)
{
    if (buff_cfg->lasting <= 0) {
        calc(to);
        to->on_buff_end(this);
        free();
        return;
    }

    buff_time += deltaTime;
    if (buff_time >= buff_cfg->lasting) {
        calc(to);
        to->on_buff_end(this);
        free();
    }
}

void BuffBase::calc(FightUnit *unit)
{
   
}

void BuffBase::free()
{

}

void BuffBase::reset()
{
    
}

