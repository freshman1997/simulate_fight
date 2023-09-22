#ifndef __AI_H__
#define __AI_H__
#include "actor.h"
#include "map.h"

class FightUnit;
class BuffBase;

/*
    ai 只包含，寻路，目标寻找、攻击、释放技能
*/
class AiBase : public Actor
{
public:
    AiBase() {}
    AiBase(FightUnit *_target) : target(_target){}
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
    bool find_path(FightUnit *);
    // 更新路径
    bool update_path(FightUnit *);

public:
    // 平 a 
    void attack(FightUnit *);
    // 移动
    void move(FightUnit *);
    // 释放技能
    void perform_skill(FightUnit *);

public:
    void set_data(FightUnit *_target)
    {
        target = _target;
    }

private:
    FightUnit *target = nullptr;
    float deltaTime = 0;
};

#endif