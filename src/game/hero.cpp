#include "game/hero.h"
#include "game/ai.h"
#include "game/buff.h"
#include "game/equip.h"
#include "game/event/event.h"
#include "manager/object_manager.hpp"
#include "game/game.h"
#include "game/fight.h"
#include "cfg/buff_cfg.h"
#include "game/skill.h"


REGISTER_TYPE("hero_base", HeroBase)

void FightUnit::do_add_buff(BuffBase *buff)
{
    buff->on_add();
    if ((buff_trigger_condition)buff->buff_cfg->trigger_cond != buff_trigger_condition::none) {
        this->trigger_buffs[(buff_trigger_condition)buff->buff_cfg->trigger_cond][buff->buff_cfg->id] = buff;
    } else {
        auto it = this->buffs.find(buff->buff_cfg->id);
        if (it != this->buffs.end()) {
            it->second->buff_time -= buff->buff_cfg->lasting;
            buff->on_remove();
            buff->free();
            return;
        } else if (buff->buff_cfg->lasting > 0) {
            this->buffs[buff->buff_cfg->id] = buff;
        }
    }

    if ((char)buff->buff_cfg->trigger_cond < 0) {
        return;
    }   

    trigger_buff(buff);
}

void FightUnit::trigger_buff(BuffBase *buff) 
{
    if (!buff) return;

    switch ((buff_func_type)buff->buff_cfg->functype) {
        case buff_func_type::hp: {
            if (!buff->params.empty()) {
                int amount = (int)buff->params[0];
                if (amount != 0) {
                    if (amount > 0) {
                        auto it = this->trigger_buffs.find(buff_trigger_condition::cure);
                        float tmp = amount;
                        for (auto &it1 : it->second) {
                            tmp += amount * it1.second->params[0];
                        }

                        this->add_hp(tmp);
                    } else {
                        this->perform_hurt(hurt_t::buff, hurt_sub_t(buff->buff_cfg->hurt_type), buff->from, buff->to, amount, buff->skill_id > 0 ? buff->skill_id : buff->id);
                    }
                }
            }
            break;
        }
        case buff_func_type::mp: {
            if (!buff->params.empty()) {
                auto it = this->trigger_buffs.find(buff_trigger_condition::mp);
                float tmp = buff->params[0];
                for (auto &it1 : it->second) {
                    tmp += buff->params[0] * it1.second->params[0];
                }
                this->add_mp(tmp);
            }
            break;
        }
        case buff_func_type::atk: {
            if (!buff->params.empty()) {
                this->add_atk_val((int)buff->params[0]);
            }
            break;
        }

        case buff_func_type::atk_def: {
            if (!buff->params.empty()) {
                this->add_ad_def_val((int)buff->params[0]);
            }
            break;
        }
        case buff_func_type::atk_dis: {
            if (!buff->params.empty()) {
                this->add_atk_distance((int)buff->params[0]);
                this->max_atk_distance += (int)buff->params[0];
            }
            break;
        }

        case buff_func_type::atk_sp: {
            if (!buff->params.empty()) {
                this->extra_atk_speed += ((int)buff->params[0]);
            }
            break;
        }

        case buff_func_type::ap_val: {
            if (!buff->params.empty()) {
                this->add_ap_val((int)buff->params[0]);
            }
            break;
        }

        case buff_func_type::ap_def: {
            if (!buff->params.empty()) {
                this->add_ap_def_val((int)buff->params[0]);
            }
            break;
        }

        case buff_func_type::silence: {
            if (this->skill) {
                this->skill->break_skill();
            }
            break;
        }

        case buff_func_type::imprison: {
            this->is_imprison = true;
            break;
        }
        case buff_func_type::strike_of_fly:
        case buff_func_type::stun: {
            if (this->skill) {
                this->skill->break_skill();
            }
            set_stuck(true);
            break;
        }
        case buff_func_type::disarm: {
            set_stuck(true);
            break;
        }
        case buff_func_type::taunt: {
            if (this->skill) {
                this->skill->break_skill();
            }

            atk_state = attack_state::prepare;
            this->ai->set_force_target(this, buff->from);
            break;
        }

        default: break;
    }
}

void FightUnit::on_buff_end(BuffBase *buff)
{
    if (!buff) {
        return;
    }

    if ((buff_trigger_condition)buff->buff_cfg->trigger_cond != buff_trigger_condition::none) {
        auto it = this->trigger_buffs.find((buff_trigger_condition)buff->buff_cfg->trigger_cond);
        if (it == this->trigger_buffs.end()) {
            return;
        }

        auto it1 = it->second.find(buff->buff_cfg->id);
        if (it1 == it->second.end()) {
            return;
        }
    } else {
        auto it = this->buffs.find(id);
        if (it == this->buffs.end()) {
            return;
        }
    }
    
    switch ((buff_func_type)buff->buff_cfg->functype) {
         case buff_func_type::silence: {
            if (this->skill) {
                this->skill->continue_skill();
            }
            break;
        }
        case buff_func_type::imprison: {
            this->is_imprison = false;
            break;
        }
        case buff_func_type::strike_of_fly:
        case buff_func_type::stun: {
            if (this->skill) {
                this->skill->continue_skill();
            }
            set_stuck(false);
            break;
        }
        case buff_func_type::disarm: {
            set_stuck(false);
            break;
        }
        case buff_func_type::taunt: {
            if (this->skill) {
                this->skill->continue_skill();
            }

            atk_state = attack_state::prepare;
            break;
        }
        default:
            break;
    }

    if (buff->buff_cfg->trigger_time == 0) {
        if ((buff_trigger_condition)buff->buff_cfg->trigger_cond != buff_trigger_condition::none) {
            this->trigger_buffs[(buff_trigger_condition)buff->buff_cfg->trigger_cond].erase(buff->buff_cfg->id);
        } else {
            this->buffs.erase(buff->buff_cfg->id);
        }

        buff->on_remove();
    }
}

void FightUnit::set_stuck(bool val)
{
    this->stuck = val;
    if (val) {
        auto it = this->trigger_buffs.find(buff_trigger_condition::stuck);
        if (it != this->trigger_buffs.end()) {
            for (auto &it1 : it->second) {
                trigger_buff(it1.second);
            }
        }
    }
}

void FightUnit::clear_stuck()
{
    this->is_imprison = false;
    this->stuck = false;
}

bool FightUnit::can_perform_skill()
{
    return skill != nullptr && skill->can_perform();
}
    
bool FightUnit::can_attack() 
{ 
    if (can_perform_skill()) {
        return false;
    }

    float dis = this->round_obj->map->distance(this);
    if (atk_state == attack_state::prepare && dis <= atk_distance && !stuck) {
        return true;
    }

    return atk_state > attack_state::prepare;
}

bool FightUnit::can_move()
{
    if (!enemy || is_imprison || can_attack() || can_perform_skill()) {
        return false;
    }

    return true;
}

void FightUnit::trigger_event(EventType type, const EventParams &params)
{
    this->round_obj->game->ev_manager->trigger_event(type, params);
}
    
void FightUnit::register_event(EventType type, int id, std::function<void (const EventParams &)> func)
{
    this->round_obj->game->ev_manager->register_event(type, id, func);
}

void FightUnit::remove_event(EventType type, int id)
{
    this->round_obj->game->ev_manager->remove_event(type, id);
}

void FightUnit::on_attack(float &damage)
{
    for (auto &it : this->equipments) {
        it.second->on_attack(damage);
    }
}

void FightUnit::on_performed_skill()
{
    for (auto &it : this->equipments) {
        it.second->on_performed_skill();
    }
}

/////////////////////////////////////
Object * HeroBase::clone()
{
    HeroBase *hero = new HeroBase;
    set_properties(hero);
    return hero;
}

Object * HeroBase::clone_and_clean()
{
    return new HeroBase;
}

HeroBase::HeroBase()
{

}

bool HeroBase::init()
{
    this->ai = dynamic_cast<AiBase *>(this->round_obj->game->create_object(hero_cfg->ai_impl_name));
    if (!this->ai) {
        return false;
    }

    atk_time = 0;
    cumulative_atk_time = 0;

    // 设置属性部分


    // 装备有些是需要计算总的属性的，所以需要最后计算
    for (auto &it : this->equipments) {
        it.second->on_begin();
    }

    if (this->owner) {
        const std::vector<BuffBase *> &player_buffs = this->owner->get_fight_buffs();
        for (auto &it : player_buffs) {

        }
    }

    return true;
}

void HeroBase::set_properties(HeroBase *)
{

}

void HeroBase::update(float deltaTime)
{
    if (stuck) return;
    
    this->delta = deltaTime;
    this->ai->update(deltaTime);
}

float HeroBase::calc_damage(hurt_sub_t type, float damage)
{
    if (type == hurt_sub_t::real) {
        return damage;
    }

    if (type == hurt_sub_t::physics) {
        // 减伤公式：当前护甲/（当前护甲+100）
        float ratio = this->ad_def_val / (this->ad_def_val + 100.0);
        return damage - damage * ratio;        
    } else if (type == hurt_sub_t::magic) {
        float ratio = this->ap_def_val / (this->ap_def_val + 100.0);
        return damage - damage * ratio;
    }

    return 0;
}

// 造成伤害的统一接口
void HeroBase::perform_hurt(hurt_t type, hurt_sub_t subtype, FightUnit *from, FightUnit *to, float damage, int actor_id)
{
    // TODO，计算护甲、魔抗、减伤、增伤、真实伤害、生命值低于 5% 斩杀 ？
    float real_damage = calc_damage(subtype, damage);
    
    if (to->shield > 0) {
        if (real_damage > to->shield) {
            real_damage -= to->shield;
            this->shield = 0;
        } else {
            to->shield -= real_damage;
            real_damage = 0;
        }
    }

    float extra_damage = 0;
    if (type == hurt_t::attack) {
        // 普工额外真实伤害
        auto it = this->trigger_buffs.find(buff_trigger_condition::atk);
        float tmp = 0;
        for (auto &it1 : it->second) {
            if (!it1.second->params.empty()) {
                tmp += calc_damage(hurt_sub_t(it1.second->buff_cfg->hurt_type), it1.second->params[0]);
            }
        }
    }

    real_damage += extra_damage;

    // 增伤、减伤、斩杀
    for (auto &it : to->trigger_buffs) {
        for (auto &it1 : it.second) {
            if ((buff_func_type)it1.second->buff_cfg->functype == buff_func_type::hp 
            && (buff_trigger_condition)it1.second->buff_cfg->trigger_cond == buff_trigger_condition::hurt 
            && (char)it1.second->buff_cfg->spec_cond >= 0) {
                if ((buff_specical_trigger_cond)it1.second->buff_cfg->spec_cond == buff_specical_trigger_cond::inc_hurt 
                    || (buff_specical_trigger_cond)it1.second->buff_cfg->spec_cond == buff_specical_trigger_cond::inc_hurt) {
                    real_damage *= it1.second->params[0];
                } else if ((buff_specical_trigger_cond)it1.second->buff_cfg->spec_cond == buff_specical_trigger_cond::behead) {
                    if (to->hp * 1.0 / to->max_hp <= it1.second->params[0]) {
                        real_damage = to->hp;
                    }
                }

                this->on_buff_end(it1.second);
            }
        }
    }

    
    if (real_damage > 0) {
        to->add_hp(std::ceil(real_damage));
        if (this->hp > 0 && this->can_perform_skill() && !this->skill->is_performing()) {
            perform_hurt_mp(real_damage);
        }
    }

    if (to->is_die()) {
        // TODO
        UnitDieParam param;
        param.die_unit = to;
        param.atk = type == hurt_t::attack;
        param.killer = from;

        trigger_event(EventType::UNIT_DIE, {to});
    }
}

void HeroBase::on_target_moved()
{

}

float HeroBase::calc_atk_speed()
{
    // 攻速如何计算 ？ 0.5 攻速表示 2 秒攻击一下
    // 攻速 = (1 + 加成攻速) * 基础攻速
    float target = (1 + extra_atk_speed) * atk_speed;
    return target / (1 / ((1 / target) * (1 / target))) * 1000;
}

void HeroBase::do_attack()
{
    float damage = calc_damage(hurt_sub_t::physics, this->atk_val);
    this->on_attack(damage);
    this->perform_hurt(hurt_t::attack, hurt_sub_t::physics, this, enemy, damage, this->id);
    if (this->hp > 0) {
        this->add_mp(10);
    }

    // 抛出攻击事件
    trigger_event(EventType::UNIT_ATTACK, {});
}

void HeroBase::attack()
{
    if (stuck) return;

    atk_time += delta;

start:
    switch (atk_state) {
        case attack_state::prepare: {
            float atk_speed = calc_atk_speed();
            if (atk_time >= atk_speed) {
                atk_state = attack_state::before;
            }
            break;
        }
        case attack_state::before: {
            int cfg_before_time = 10;
            if (delta >= cfg_before_time) {
                atk_state = attack_state::after;
                cumulative_atk_time = delta - cfg_before_time;
                goto start;
            } 

            if (atk_time >= cfg_before_time) {
                atk_state = attack_state::after;
            }
            break;
        }
        case attack_state::after: {
            do_attack();
            atk_state = attack_state::prepare;
            atk_time = 0;
            if (cumulative_atk_time > 0) {
                atk_time = cumulative_atk_time;
                goto start;
            }
            break;
        }
        default:break;
    }
}

void HeroBase::on_being_attack(FightUnit *from, float damage)
{
    // 增加蓝量、调用装备的接口
    this->perform_hurt(hurt_t::attack, hurt_sub_t::physics, from, this, damage, from->id);

    // 抛出被攻击事件
    trigger_event(EventType::UNIT_HURT, {});
}

void HeroBase::buff_action()
{
    for (auto it = buffs.begin(); it != buffs.end(); ++it) {
        it->second->update(delta);
    }
}

void HeroBase::perform_hurt_mp(int damage)
{
    // y = 0.08 * damage => 回蓝量，damage 为被攻击后失去生命值，包括普工、技能等方式
    int add_amount = (int)(0.08 * damage);
    this->add_mp(add_amount);
}

