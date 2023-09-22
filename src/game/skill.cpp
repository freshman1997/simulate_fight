#include <algorithm>
#include "game/skill.h"
#include "game/buff.h"
#include "game/fight.h"
#include "game/game.h"
#include "game/ai.h"
#include "manager/cfg_manager.h"
#include "manager/object_manager.hpp"
#include "game/lottery.h"

REGISTER_TYPE("skill_base", SkillBase)

Object * SkillBase::clone()
{
    return new SkillBase;
}

Object * SkillBase::clone_and_clean()
{
    return clone();
}

void SkillBase::deinit()
{
    skill_cfg = nullptr;
    owner = nullptr;
    state = skill_state::prepare;
    start_time = 0;
    singing_time = 0;
    skill_buffs.clear();
    skill_buffs.shrink_to_fit();
    targets.clear();
    targets.shrink_to_fit();
}

void SkillBase::update(float deltaTime)
{
    // 这个标志由外部修改
    if (state == skill_state::stuck) {
        return;
    }

    if (!can_perform()) {
        return;
    }

    if (state == skill_state::prepare) {
        state = skill_state::singing;
    }

    if (state == skill_state::singing) {
        singing_time += deltaTime;
        if (singing_time >= skill_cfg->skill_sing_time) {
            state = skill_state::performing;
            singing_time = singing_time - skill_cfg->skill_sing_time;
            if (!find_targets()) {
                on_end();
                return;
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

        if (start_time >= skill_cfg->skill_time) {
            perform_skill(deltaTime);
            on_end();
        }
    }
}

bool SkillBase::can_perform()
{
    if (this->state == skill_state::prepare) { 
        if ((skill_perform_type)skill_cfg->perform_type == skill_perform_type::mp) {
            return owner->mp >= owner->hero_cfg->mp[owner->star];
        }
    }

    return this->state > skill_state::prepare && this->state < skill_state::stuck;
}

bool SkillBase::find_targets()
{
    int amount = this->skill_cfg->target_amount;
    switch ((skill_target_choose_rule)this->skill_cfg->target_choose_rule) {
        case skill_target_choose_rule::round_rand: {
            FightUnit *unit = this->owner->round_obj->map->rand_around_one(this->skill_cfg->distance, owner->enemy->pos);
            if (unit) {
                targets.push_back(unit);
            }
            break;
        }
        case skill_target_choose_rule::round_rand_nearest: {
            if (amount < 0) return false;

            std::vector<FightUnit *> units = this->owner->round_obj->map->find_around(this->skill_cfg->distance, owner->enemy->pos);
            if (!units.empty()) {
                std::sort(units.begin(), units.end(), [this](const FightUnit *a, const FightUnit *b) ->bool {
                    return owner->round_obj->map->distance(a, owner) < owner->round_obj->map->distance(b, owner);
                });

                std::copy(units.begin(), units.begin() + amount, targets.begin());
            }
            break;
        }
        case skill_target_choose_rule::round_rand_farthest: {
            if (amount < 0) return false;

            std::vector<FightUnit *> units = this->owner->round_obj->map->find_around(this->skill_cfg->distance, owner->enemy->pos);
            if (!units.empty()) {
                std::sort(units.begin(), units.end(), [this](const FightUnit *a, const FightUnit *b) ->bool {
                    return owner->round_obj->map->distance(a, owner) > owner->round_obj->map->distance(b, owner);
                });

                std::copy(units.begin(), units.begin() + amount, targets.begin());
            }
            break;
        }
        case skill_target_choose_rule::round_all: {
            const std::vector<FightUnit *> &units = this->owner->round_obj->map->find_around(this->skill_cfg->distance, owner->enemy->pos);
            if (units.empty()) {
                return false;
            }

            amount = amount < 0 ? units.size() : amount;
            std::copy(units.begin(), units.begin() + amount, targets.begin());
            break;
        }
        case skill_target_choose_rule::cur_target: {
            this->targets.push_back(this->owner->enemy);
            break;
        }
        case skill_target_choose_rule::round_mostest_unit: {

            break;
        }
        case skill_target_choose_rule::round_hp_lowest: {
            if (amount < 0) return false;

            std::vector<FightUnit *> units = this->owner->round_obj->map->find_around(this->skill_cfg->distance, owner->enemy->pos);
            if (!units.empty()) {
                std::sort(units.begin(), units.end(), [this](const FightUnit *a, const FightUnit *b) ->bool {
                    return owner->round_obj->map->distance(a, owner) > owner->round_obj->map->distance(b, owner);
                });

                amount = amount < 0 ? units.size() : amount;
                std::copy(units.begin(), units.begin() + amount, targets.begin());
            }

            break;
        }
        case skill_target_choose_rule::target_dir: {
            const std::vector<FightUnit *> &units = this->owner->round_obj->map->dir_units(owner);
            if (!units.empty()) {
                amount = amount < 0 ? units.size() : amount;
                std::copy(units.begin(), units.begin() + amount, targets.begin());
            }
            break;
        }

        default: break;
    }
    return false;
}

void SkillBase::perform_skill(float deltaTime)
{
    // 技能通用逻辑
    if (!can_perform() || targets.empty()) {
        return;
    }

    if ((skill_perform_type)skill_cfg->perform_type == skill_perform_type::atk_3) {
        return;
    }

    build_buffs();
    transfor_buffs();
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
    start_time = 0;
    singing_time = 0;
    // TODO
}

void SkillBase::continue_skill()
{
    if (state != skill_state::stuck) return;

    state = skill_state::prepare;
    // TODO

}

void SkillBase::transfor_buffs()
{
    for (auto &it : targets) {
        if (it->is_die()) continue;
        
        for (auto &buff : skill_buffs) {
            it->add_buff(buff);
        }
    }
}

void SkillBase::build_buffs()
{
    if (targets.empty()) {
        return;
    }

    int star = this->owner->star;
    auto it = this->skill_cfg->buffs.find(star);
    if (it == this->skill_cfg->buffs.end()) {
        return;
    }

    for (auto &it1 : it->second) {
        auto buff_cfg = CfgManager::get_instance().buff_cfg.get_buff(it1.buff_id);
        if (!buff_cfg) {
            continue;
        }

        BuffBase *buff =  (BuffBase *)this->owner->round_obj->game->create_object(buff_cfg->impl_name);
        if (!buff) {
            continue;
        }

        buff->buff_cfg = buff_cfg;
        buff->f_type = buff_from_type::skill;
        buff->from = owner;
        buff->params = it1.params;
        buff->skill_id = this->id;
        if (this->owner->skill_critic) {
            buff->set_buff_critical_param();
        }

        this->skill_buffs.push_back(buff);
    }
}
