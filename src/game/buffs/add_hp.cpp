#include "game/buffs/add_hp.h"
#include "game/fight.h"

AddHpBuff::AddHpBuff() : BaseBuff(buff_type::hp), add_amount(0)
{}

void AddHpBuff::calc(FightUnit *unit)
{
    unit->add_hp(this->add_amount);
}
