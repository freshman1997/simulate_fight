#ifndef __AI_H__
#define __AI_H__
#include "actor.h"
#include "map.h"

class FightUnit;
class BuffBase;

enum class hurt_t
{
    attack,
    buff,
    skill,
};

enum class hurt_sub_t
{
    normal,
    real,

};

enum class action_state
{
    none = -1,
    attack,
    move,
    skill,
};

/*
    ai 只包含，寻路，目标寻找、攻击、释放技能
*/
class AiBase : public Actor
{
public:
    virtual void update(float deltaTime);
    virtual Object * clone() { return new AiBase; };
    virtual Object * clone_and_clean() { return clone(); };
    
public:
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

public:
    // 造成伤害的统一接口
    void perform_hurt(hurt_t, hurt_sub_t, FightUnit *from, FightUnit *to, float demage);
    // 平 a 
    void attack(FightUnit *);
    // 移动
    void move(FightUnit *);
    // 释放技能
    void perform_skill(FightUnit *);

public:
    void change_state(action_state _state) { this->_state = _state; }

private:
    action_state _state = action_state::none;
    FightUnit *target = nullptr;
    GameMap *map = nullptr;
    float deltaTime = 0;
};

#endif