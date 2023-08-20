#ifndef __KUANG_FENG_JUE_XI_ZHAN_H__
#define __KUANG_FENG_JUE_XI_ZHAN_H__
#include "game/skill.h"

/**
 * @brief 狂风绝息斩，s9 亚索技能， 4费卡
 * 朝3格内相距最远的那个敌人刮出一道飓风，
 * 对命中的所有敌人造成击飞和晕眩效果。冲刺并斩击原目标，
 * 从而对其造成425/640/2835物理伤害（500/500/1500%+55/85/300%）。
 * 将其砸到地上然后对1格内的敌人们造成225/339/1267物理伤害（300%/300%/750%）。 
 * 艾欧尼亚加成：+15%全能吸血
 */

class KuangFengJueXiZhan : public SkillBase
{
private:
    enum class performing_state
    {
        release_wind,       // 释放龙卷风
        wind_move,          // 龙卷风移动，可以创建一个物体移动，独立管理移动和碰撞部分，回调回来
        sprint,             // 冲刺到目标位置
        slash,              // 斩击目标、造成伤害
    };

    performing_state p_state = performing_state::release_wind;

public:
    virtual void perform(float deltaTime, bool first = false);
    virtual bool find_targets();
    virtual std::vector<BuffBase *> build_buffs();
    
    virtual Object * clone();
    virtual Object * clone_and_clean();

private:
    void on_wind_hit(FightUnit *target);
    int slash_time = 0;
};

#endif
