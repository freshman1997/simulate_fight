#ifndef __TIAN_BENG_DI_LIE_H__
#define __TIAN_BENG_DI_LIE_H__
#include "game/skill.h"
/**
 * @brief s9 皇子技能 天崩地裂
 * 跃到附近一名敌人身上。对2格内的敌人们造成140/210/800点魔法伤害和1.75/2.25/8秒晕眩。
 */
class TianBengDiLie : public SkillBase
{
public:
    virtual void perform_skill(float deltaTime, bool first = false);
    
    virtual Object * clone();
    virtual Object * clone_and_clean();
};

#endif