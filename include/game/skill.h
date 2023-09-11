#ifndef __SKILL_H__
#define __SKILL_H__
#include <unordered_map>

#include "actor.h"
#include "cfg/skill_cfg.h"

class FightUnit;
class BuffBase;

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
    round_rand_1,           // 周围一格范围内
    round_rand_2,           // 周围两个范围内
    most_1,                 // 人数最多的 1 格内
    most_2,                 // 人数最多的 2 格内
    farthest,               // 最远的敌人
};

enum class skill_target_sort_type : char
{
    none = -1,
};

class SkillBase : public Actor
{
public:
    virtual void update(float deltaTime);
    virtual void break_skill();
    virtual void continue_skill();
    virtual Object * clone() = 0;
    virtual Object * clone_and_clean() = 0;

public:
    void perform_skill(float deltaTime, bool first = false);
    bool is_performing() { return state == skill_state::performing; }
    bool is_break() { return state == skill_state::stuck; }
    virtual bool can_perform();

protected:
    virtual void perform(float deltaTime, bool first = false);
    virtual bool find_targets();
    virtual void on_end();
    virtual std::vector<BuffBase *> build_buffs();
    virtual void send_buff();

public:
    const Skill * skill_cfg = nullptr;
    FightUnit *owner = nullptr;

protected:
    skill_state state = skill_state::prepare;
    int start_time = 0;
    int singing_time = 0;
    std::vector<FightUnit *> targets;
};

#endif