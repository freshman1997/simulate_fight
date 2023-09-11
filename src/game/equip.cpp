#include "game/equip.h"

Object * EquipBase::clone()
{
    return new EquipBase;
}

Object * EquipBase::clone_and_clean()
{
    return clone();
}

void EquipBase::update(float deltaTime)
{
    
}

// 进入战斗前计算
void EquipBase::on_begin()
{

}

// 受到伤害时
void EquipBase::on_being_hurt(int damage, int type)
{

}