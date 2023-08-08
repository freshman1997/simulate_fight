#include "game/ai.h"

// 平 a 
void AiBase::attack(FightUnit *)
{

}

// 移动
bool AiBase::move(FightUnit *)
{
    return false;
}

// 释放技能
void AiBase::perform_skill(FightUnit *)
{

}

// 更新目标
void AiBase::update_target(FightUnit *)
{

}

// 寻找目标
FightUnit * AiBase::find_target(FightUnit *)
{
    return nullptr;
}

// 强制设置目标，如嘲讽
void AiBase::set_force_target(FightUnit *unit, FightUnit *target)
{

}

// 寻路
void AiBase::find_path(FightUnit *)
{

}

// 更新路径
void AiBase::update_path(FightUnit *)
{
    
}

// 目标消失
bool AiBase::is_target_miss()
{
    return false;
}

// 能否攻击
bool AiBase::can_attack(FightUnit *)
{
    return false;
}

// 能否移动
bool AiBase::can_move(FightUnit *)
{
    return false;
}

// 是否被控制住了，击飞，眩晕，禁锢
bool AiBase::is_control(FightUnit *)
{
    return false;
}

// 是否可以释放技能
bool AiBase::can_perform_skill(FightUnit *)
{
    return false;
}
