#include "game/buffs/add_hp.h"
#include "game/fight.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("add_hp_buff", AddHpBuff);

Object * AddHpBuff::clone()
{
    return new AddHpBuff();
}

Object * AddHpBuff::clone_and_clean()
{
    return new AddHpBuff();
}

AddHpBuff::AddHpBuff() : BaseBuff(buff_type::hp), add_amount(0)
{}

void AddHpBuff::calc(FightUnit *unit)
{
    unit->add_hp(this->add_amount);
}
