#include "game/skills/KuangFengJueXiZhan.h"
#include "manager/object_manager.hpp"

REGISTER_TYPE("kuang_feng_jue_xi_zhan", KuangFengJueXiZhan)

Object * KuangFengJueXiZhan::clone()
{
    return new KuangFengJueXiZhan;
}

Object * KuangFengJueXiZhan::clone_and_clean()
{
    return clone();
}

bool KuangFengJueXiZhan::find_targets()
{
    return false;
}

std::vector<BuffBase *> KuangFengJueXiZhan::build_buffs()
{
    return {};
}

void KuangFengJueXiZhan::on_wind_hit(FightUnit *target)
{
    targets.push_back(target);
}

void KuangFengJueXiZhan::perform(float deltaTime, bool first)
{
    switch (p_state)
    {
    case performing_state::release_wind: {
        // 创建一个龙卷风对象并放到场景、地图中
        break;
    }
    case performing_state::wind_move: {
        // do nothing，等待
        break;
    }
    case performing_state::sprint: {
        // 冲刺
        break;
    }
    case performing_state::slash: {
        // 斩击动画，动画结束造成伤害
        slash_time += deltaTime;
        int cfg_slash_time = 100;
        if (slash_time >= cfg_slash_time) {
            // TODO 造成2段伤害
            p_state = performing_state::release_wind;
            slash_time = 0;
        } 
        break;
    }

    default:
        break;
    }
}



