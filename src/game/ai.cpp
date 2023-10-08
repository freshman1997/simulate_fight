#include "game/ai.h"
#include "game/actor.h"
#include "game/hero.h"
#include "game/skill.h"
#include "game/fight.h"
#include "game/game.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("ai_base", AiBase)

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
    target->buff_action();
    
    if (target->can_perform_skill()) {
        perform_skill(target);
    } else if (target->can_attack()) {
        attack(target);
    } else if (target->can_move()) {
        move(target);
    }
}

// 平 a 
void AiBase::attack(FightUnit *unit)
{
    if (!unit) return;

    FightUnit *enemy = unit->enemy;
    if (!enemy || unit->can_attack()) return;

    unit->attack();
}

// 移动
void AiBase::move(FightUnit *unit)
{
    if (!this->update_path(unit) || unit->path.empty() 
        || unit->move_step < 0 || unit->move_step >= unit->path.size()) {
        return;
    }

    float incDest = unit->move_speed * 1.0 / this->target->round_obj->game->cur_frame_time;
    unit->move_distance += incDest;

    const Vector2 &next_step = unit->path[unit->move_step];
    if (target->round_obj->map->vector2_distance(unit->pos, next_step) > unit->move_distance) {
        return;
    }

    
    if (!target->round_obj->map->move(unit, next_step)) {
        return;
    }

    ++unit->move_step;
    unit->move_distance = 0l;
    unit->pos = next_step;
    // TODO
}

// 释放技能
void AiBase::perform_skill(FightUnit *unit)
{
    if (!unit->can_perform_skill()) return;

    unit->skill->update(this->deltaTime);
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
    if (unit->is_die()) {
        return;
    }

    unit->enemy = target;
    // TODO
}

// 寻路
bool AiBase::find_path(FightUnit *unit)
{
    if (!unit || !unit->enemy || unit->path.empty()) {
        return false;
    }

    std::vector<Vector2> &path = unit->path;
    path.clear();

    target->round_obj->map->find_path(unit->pos, unit->enemy->pos, path, unit->atk_distance);
    if (path.empty()) {
        return false;
    }

    return true;
}

// 更新路径
bool AiBase::update_path(FightUnit *unit)
{
    // 这里目标消失会被修改
    bool change_target = false;
    if (!unit->enemy) {
        unit->enemy = find_target(unit);
        change_target = true;
    } else {
        if (unit->enemy->is_die()) {
            update_target(unit);
            change_target = true;
        }
    }

    if (unit->path.empty() || change_target) {
        bool ret = find_path(unit);
        if (!ret) {
            if (!unit->path.empty()) {
                // TODO 清空路径
                unit->path.clear();
            } 
            return false;
        } else {
            unit->move_distance = 0l;
            unit->move_step = 1;
        }
    }

    return true;
}
