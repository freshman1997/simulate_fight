#ifndef __SKILL_H__
#define __SKILL_H__

#include "actor.h"

class FightUnit;

enum class SkillType
{
    active,         // 主动
    positive        // 被动
};

class SkillBase : public Actor
{
public:
    SkillType get_type();
    virtual SkillBase * clone() = 0;
    virtual void perform_skill() = 0;

public:
    SkillType type;
    int skid;
    FightUnit *owner;
};

#endif