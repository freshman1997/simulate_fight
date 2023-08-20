#include "game/buffs/add_hp.h"
#include "game/fight.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("add_hp_buff", AddHpBuff);

AddHpBuff::AddHpBuff() : add_amount(0)
{}

Object * AddHpBuff::clone()
{
    return new AddHpBuff();
}

Object * AddHpBuff::clone_and_clean()
{
    return new AddHpBuff();
}

void AddHpBuff::calc(FightUnit *unit)
{
    unit->add_hp(this->add_amount);
}
