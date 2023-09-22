#include "game/fetters/knoxus.h"
#include "game/player.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("knoxus", Knoxus)

Object * Knoxus::clone()
{
    return new Knoxus;
}

Object * Knoxus::clone_and_clean()
{
    return clone();
}

void Knoxus::on_before_round()
{

}

void Knoxus::update(float delta)
{

}

void Knoxus::on_unit_die(FightUnit *killer, FightUnit *unit)
{
    if (!this->player || !killer || !unit) {
        return;
    }

    if (this->player->player_id != killer->owner->player_id) {
        return;
    }

    
}
