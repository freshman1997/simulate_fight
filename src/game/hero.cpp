#include "game/hero.h"
#include "game/ai.h"
#include "game/equip.h"
#include "game/event/event.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("hero_base", HeroBase)

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
    init();
}

void HeroBase::init()
{
    atk_time = 0;
    cumulative_atk_time = 0;
    for (auto &it : this->equipments) {
        it.second->on_begin();
    }
}

void HeroBase::set_properties(HeroBase *)
{

}

void HeroBase::update(float deltaTime)
{
    this->delta = deltaTime;
    this->ai->update(deltaTime);
}

void HeroBase::move()
{

}

float HeroBase::calc_atk_speed()
{
    // 攻速如何计算 ？ 0.5 攻速表示 2 秒攻击一下
    // 攻速 = (1 + 加成攻速) * 基础攻速
    float extra =  0.5;
    float res = (1 + extra) * atk_speed;
    return atk_speed / 1000;
}

void HeroBase::do_attack()
{
    // this->ai->perform_hurt();

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

void HeroBase::on_being_attack()
{
    // TODO 增加蓝量、调用装备的接口、

}

void HeroBase::buff_action()
{
    for (auto it = buffs.begin(); it != buffs.end(); ++it) {
        (it->second)->update(delta);
    }
}

void HeroBase::on_buff_end(BuffBase *buff)
{

}


