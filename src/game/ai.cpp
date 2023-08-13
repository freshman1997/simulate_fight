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

void AiBase::update(float deltaTime)
{
    this->deltaTime = deltaTime;
    buff_action(target);
    
    switch (_state)
    {
    case action_state::attack:
        attack(target);
        break;
    case action_state::move:
        move(target);
        break;
    case action_state::skill:
        perform_skill(target);
        break;
    default:
        break;
    }
}

// 造成伤害的统一接口
void AiBase::perform_hurt(hurt_t type, hurt_sub_t subtype, FightUnit *from, FightUnit *to, float demage)
{
    // TODO，计算护甲、魔抗、减伤、增伤、真实伤害、

}

// 平 a 
void AiBase::attack(FightUnit *unit)
{
    if (!unit) return;

    FightUnit *enemy = unit->enemy;
    if (!enemy) return;

    unit->attack();
}

// 移动
void AiBase::move(FightUnit *unit)
{
    // 这里目标消失会被修改
    if (!unit->enemy) {
        unit->enemy = find_target(unit);
    } else {
        if (unit->enemy->is_die()) {
            update_target(unit);
        }
    }

    if (!unit->path) {
        find_path(unit);
    }

    if (unit->skill) {
        perform_skill(unit);
    } else {
        // TODO 路径变化怎么体现 ？
        update_path(unit);
    }

    float dis = map->distance(unit);
    if (unit->atk_distance <= dis) {
        attack(unit);
    }

    unit->move();
}

// 释放技能
void AiBase::perform_skill(FightUnit *unit)
{

}

// 生效buff
void AiBase::buff_action(FightUnit *unit)
{

}

//---------------- helper ----------------//

// 更新目标
void AiBase::update_target(FightUnit *unit)
{

}

// 寻找目标
FightUnit * AiBase::find_target(FightUnit *unit)
{
    return nullptr;
}

// 强制设置目标，如嘲讽
void AiBase::set_force_target(FightUnit *unit, FightUnit *target)
{

}

// 寻路
void AiBase::find_path(FightUnit *unit)
{

}

// 更新路径
void AiBase::update_path(FightUnit *unit)
{
    
}
