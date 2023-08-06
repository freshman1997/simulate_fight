#ifndef __SKILL_H__
#define __SKILL_H__

#include "actor.h"

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

public:
    SkillType type;
    int skid;
};

#endif