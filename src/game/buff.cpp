#include "game/buff.h"
#include "cfg/buff_cfg.h"
#include "manager/object_manager.hpp"
#include "game/hero.h"

REGISTER_TYPE("buff_base", BuffBase)

Object * BuffBase::clone()
{
    return new BuffBase;
}

Object * BuffBase::clone_and_clean()
{
    return clone();
}

void BuffBase::update(float deltaTime)
{
    if (buff_cfg->lasting <= 0) {
        to->on_buff_end(this);
        free();
        return;
    }

    buff_time += deltaTime;
    if (buff_time >= buff_cfg->lasting) {
        to->on_buff_end(this);
        free();
    }
}

void BuffBase::on_add() 
{ 
    buff_time = 0;
    if (!this->to) {
        return;
    }

    trigger_time = buff_cfg->trigger_time;
    if ((buff_param_type)this->buff_cfg->param_type == buff_param_type::percent) {
        switch ((buff_func_type)this->buff_cfg->functype) {
            case buff_func_type::hp: {
                if (!params.empty()) {
                    float real_amount = this->to->max_hp * params[0];
                    params.insert(params.begin(), real_amount);
                }
            }
            case buff_func_type::mp: {
                if (!params.empty()) {
                    float real_amount = this->to->max_mp * params[0];
                    params.insert(params.begin(), real_amount);
                }
            }
            default: break;
        }
    }

    to->trigger_event(EventType::UNIT_ADD_BUFF, {this});
}

void BuffBase::on_remove() 
{ 
    --trigger_time;
    reset();
    to->trigger_event(EventType::UNIT_RM_BUFF, {this});
}

void BuffBase::free()
{
    if (trigger_time <= 0) {
        ObjectManager::get_instance().release_object(this);
    }
}

void BuffBase::reset()
{
    if (trigger_time <= 0) {
        params.clear();
    }
}

void BuffBase::set_buff_critical_param()
{
    if (!this->from || !this->from->skill_critic) {
        return;
    }

    critical = true;
}

