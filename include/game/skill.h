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

protected:
    virtual void perform(float deltaTime, bool first = false);
    virtual bool find_targets();
    virtual void on_end();
    virtual bool can_perform();
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