#ifndef __SKILL_H__
#define __SKILL_H__

#include "actor.h"

class FightUnit;

enum class skill_type : char
{
    none = -1,
    active,         // 主动
    positive        // 被动
};

enum class skill_perform_type : char
{
    none = -1,
    
};

class SkillBase : public Actor
{
public:
    virtual SkillBase * clone() = 0;
    virtual void update(float deltaTime);
    virtual void perform_skill();
    virtual void break_skill();
    virtual void continue_skill();
    skill_type get_type() { return type; }

protected:
    bool is_performing();
    bool is_break() { return is_stuck; }

public:
    skill_type type = skill_type::none;
    int skid = 0;
    FightUnit *owner = nullptr;
    float sing_time = 0;
    float skill_time = 0;

protected:
    float start_time = 0;
    float singing_time = 0;
    bool start_skill = false;
    bool start_singing = false;
    bool is_stuck = false;
};

#endif