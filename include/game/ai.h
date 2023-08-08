﻿#ifndef __AI_H__
#define __AI_H__
#include "actor.h"
#include "map.h"

class FightUnit;

/*
    ai 只包含，寻路，目标寻找、攻击、释放技能
*/
class AiBase : public Actor
{
public:
    // 平 a 
    void attack(FightUnit *);
    // 移动
    bool move(FightUnit *);
    // 释放技能
    void perform_skill(FightUnit *);
    // 更新目标
    void update_target(FightUnit *);
    // 寻找目标
    FightUnit * find_target(FightUnit *);
    // 强制设置目标，如嘲讽
    void set_force_target(FightUnit *, FightUnit *target);
    // 寻路
    void find_path(FightUnit *);
    // 更新路径
    void update_path(FightUnit *);

private:
    // 目标消失
    bool is_target_miss();
    // 能否攻击
    bool can_attack(FightUnit *);
    // 能否移动
    bool can_move(FightUnit *);
    // 是否被控制住了，击飞，眩晕，禁锢
    bool is_control(FightUnit *);
    // 是否可以释放技能
    bool can_perform_skill(FightUnit *);

private:
    FightUnit *target;
    GameMap *map;
};

#endif