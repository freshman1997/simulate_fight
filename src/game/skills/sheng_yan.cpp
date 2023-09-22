#include "game/skills/sheng_yan.h"
#include "game/event/event.h"
#include "game/hero.h"
#include "game/skill.h"
#include "manager/object_manager.hpp"
#include "nlohmann/json_fwd.hpp"
#include <string>

REGISTER_TYPE("sheng_yan", ShengYan)

ShengYan::ShengYan() : cur_cumulative_hp(0)
{}

Object * ShengYan::clone()
{
    return new ShengYan;
}

Object * ShengYan::clone_and_clean()
{
    return clone();
}

bool ShengYan::check_extra_config()
{
    if (!this->skill_cfg) {
        return false;
    }

    const json &js = this->skill_cfg->extra_params;
    if (js.empty() || js.size() != 3) {
        return false;
    }

    for (auto &it : js.items()) {
        if (it.value() <= 0) {
            return false;
        }
    }

    return true;
}

bool ShengYan::init()
{
    if (!check_extra_config()) {
        return false;
    }

    cur_cumulative_hp = 0;

    this->owner->register_event(EventType::UNIT_DIE, this->id, [this](const EventParams &param) {
        if (!param.ptr) {
            return;
        }

        UnitDieParam *ev_param = reinterpret_cast<UnitDieParam *>(param.ptr);
        if (!ev_param || ev_param->actor_id != this->id) {
            return;
        }

        this->on_kill_unit();
    });

    return true;
}

void ShengYan::deinit()
{
    this->owner->remove_event(EventType::UNIT_DIE, this->id);
    SkillBase::deinit();
}

void ShengYan::on_begin()
{
    if (!this->owner) {
        return;
    }

    this->owner->add_hp(this->cur_cumulative_hp);
}

void ShengYan::on_kill_unit()
{
    if (!this->owner || !this->skill_cfg) {
        return;
    }

    int star = this->owner->star;
    const json &js = this->skill_cfg->extra_params;
    int add_hp = js[std::to_string(star)];
    this->cur_cumulative_hp += add_hp;
}

