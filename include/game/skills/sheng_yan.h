#ifndef __SKILL_SHENG_YAN_H__
#define __SKILL_SHENG_YAN_H__

#include "game/skill.h"
/*
    盛宴（虚空恐惧 科加斯）

    对距离内生命值最低的敌人造成355/524/790魔法伤害。 
    如果这个技能击杀了该敌人，那么会将其吞噬并永久获得30/35/40最大生命值
*/

class ShengYan : public SkillBase
{
public:
    ShengYan();
    virtual Object * clone();
    virtual Object * clone_and_clean();

    virtual bool init();
    virtual void deinit();
    virtual void on_begin();
    virtual bool check_extra_config();

public:
    void on_kill_unit();

private:
    int cur_cumulative_hp;
};

#endif // !__SKILL_SHENG_YAN_H__