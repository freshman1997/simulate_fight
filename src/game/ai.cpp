#include "game/ai.h"
#include "game/hero.h"
#include "game/skill.h"

// 战斗怎么做？
/*
    1、先寻找目标
    2、检查攻击距离，不够则寻路过去
    3、普攻攻击并产生蓝量、蓝量满了释放技能
    4、目标死亡切换回到 1
    5、如果没办法攻击，如被占满外围位置了，则不动等待其他棋子攻击
*/

// 平 a 
void AiBase::attack(FightUnit *unit)
{

}

// 移动
bool AiBase::move(FightUnit *unit)
{
    if (!unit->enemy) {
        unit->enemy = find_target(unit);
    }

    if (!unit->path) {
        find_path(unit);
    }

    float consum = 0;
    if (unit->skill) {
        unit->skill->perform_skill();
    }

    float dis = map->distance(unit);
    if (unit->atk_distance <= dis) {
        // 应该调用具体英雄或者野怪平A
        unit->attack();
    }

    // 根据移速计算移动到目的地需要的时间
    

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
