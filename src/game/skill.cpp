#include "game/skill.h"
#include "game/fight.h"
#include "game/game.h"
#include "game/ai.h"

void SkillBase::update(float deltaTime)
{
    // 这个标志由外部修改
    if (state == skill_state::stuck) {
        start_time = 0;
        singing_time = 0;
        return;
    }

    if (!can_perform()) {
        return;
    }

    if (state == skill_state::prepare) {
        state = skill_state::singing;
    }

    bool first_perform = false;
    if (state == skill_state::singing) {
        singing_time += deltaTime;
        if (singing_time >= skill_cfg->skill_sing_time) {
            state = skill_state::performing;
            singing_time = singing_time - skill_cfg->skill_sing_time;
            first_perform = true;
            if (!find_targets()) {
                // TODO
            }
        }
    }

    if (state == skill_state::performing) {
        if (singing_time > 0) {
            start_time += singing_time + deltaTime;
            singing_time = 0;
        } else {
            start_time += deltaTime;
        }

        perform_skill(deltaTime, first_perform);

        if (start_time >= skill_cfg->skill_time) {
            on_end();
        }
    }
}

bool SkillBase::can_perform()
{
    if (this->state == skill_state::prepare && (skill_perform_type)skill_cfg->perform_type == skill_perform_type::mp) {
        return owner->mp >= owner->hero_cfg->mp[owner->star];
    }  

    return false;
}

bool SkillBase::find_targets()
{
    return false;
}

void SkillBase::perform(float deltaTime, bool first)
{
    if (!first) {
        return;
    }
    
    send_buff();
}

void SkillBase::perform_skill(float deltaTime, bool first)
{
    if (first) {
        this->owner->ai->change_state(action_state::skill);
    }

    perform(deltaTime, first);
}

void SkillBase::on_end()
{
    start_time = 0;
    state = skill_state::prepare;
    this->targets.clear();
    this->owner->on_skill_end();
}

void SkillBase::break_skill()
{   
    state = skill_state::stuck;
    // TODO
}

void SkillBase::continue_skill()
{
    if (state != skill_state::stuck) return;

    state = skill_state::prepare;
    // TODO

}

std::vector<BuffBase *> SkillBase::build_buffs()
{
    return {};
}

void SkillBase::send_buff()
{
    const std::vector<BuffBase *> &buffs = build_buffs();
    if (buffs.empty()) return;
    
    for (auto &it : this->targets) {
        for (auto &buff : buffs) {
            it->add_buff(buff);
        }
    }
}
