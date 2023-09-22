#ifndef __SKILL_H__
#define __SKILL_H__
#include <unordered_map>

#include "actor.h"
#include "cfg/skill_cfg.h"

class FightUnit;
class BuffBase;

enum class skill_state
{
    prepare,
    singing,
    performing,
    stuck,
};

enum class skill_target_choose_rule : char
{
    none = 0,
    round_rand,                 // 范围内随机
    round_rand_nearest,         // 范围内最近       
    round_rand_farthest,        // 范围内最远的敌人
    round_all,                  // 范围内所有
    cur_target,                 // 当前目标
    round_mostest_unit,         // 范围内最多单位区域
    round_hp_lowest,            // 范围内血量最少
    target_dir,                 // 与目标同一条直线上
};

enum class skill_type : int
{
    none = -1,
    active,         // 主动
    positive        // 被动
};

enum class skill_perform_type : int
{
    none = -1,
    mp,         // 蓝量满了
    atk_3,      // 攻击第三下
};

enum class skill_target_sort_type : char
{
    none = -1,
    distance,               // 距离
    damage,                 // 单位当前输出伤害
};

class SkillBase : public Actor
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();
    virtual void deinit();
    virtual void update(float deltaTime);
    virtual void break_skill();
    virtual void continue_skill();
    virtual bool check_extra_config() { return true; }

public:
    virtual void perform_skill(float deltaTime);
    bool is_performing() { return state == skill_state::performing; }
    bool is_break() { return state == skill_state::stuck; }
    virtual bool can_perform();

    // 进场之前被调用，用于添加技能累积的buff等
    virtual void on_begin() {}

protected:
    bool find_targets();
    virtual void on_end();
    // 创建buff列表，包括释放就添加的、累积的其他buff
    virtual void build_buffs();

    // 施加buff到目标单位上
    virtual void transfor_buffs();

public:
    const Skill * skill_cfg = nullptr;
    FightUnit *owner = nullptr;

protected:
    skill_state state = skill_state::prepare;
    int start_time = 0;
    int singing_time = 0;
    std::vector<BuffBase *> skill_buffs;
    std::vector<FightUnit *> targets;
};

#endif