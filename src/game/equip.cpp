#include "game/equip.h"
#include "game/buff.h"
#include "game/event/event.h"
#include "game/hero.h"
#include "cfg/equip_cfg.h"
#include "game/object.h"
#include "manager/cfg_manager.h"
#include "game/fight.h"
#include "game/game.h"
#include "manager/object_manager.hpp"
#include <algorithm>

REGISTER_TYPE("equip_base", EquipmentBase)

Object * EquipmentBase::clone()
{
    return new EquipmentBase;
}

Object * EquipmentBase::clone_and_clean()
{
    return clone();
}

void EquipmentBase::update(float deltaTime)
{
    // do nothing
}

// 进入战斗前计算
bool EquipmentBase::init()
{
    if (!equip_cfg || !owner) {
        return false;
    }

    for (auto &it : equip_cfg->buffs) {
        const Buff *buff_cfg = CfgManager::get_instance().buff_cfg.get_buff(it.first);
        if (!buff_cfg) {
            continue;
        }

        Object *obj = this->owner->round_obj->game->create_object(buff_cfg->impl_name);
        if (!obj) {
            continue;
        }

        BuffBase *buff = dynamic_cast<BuffBase *>(obj);
        if (!buff) {
            continue;
        }

        buff->buff_cfg = buff_cfg;
        buff->to = this->owner;
        if (!it.second.empty()) {
            buff->lasting = it.second[0];
            std::copy(it.second.begin() + 1, it.second.end(), buff->params.begin());
        }
        
        buff->trigger_time = buff_cfg->trigger_time;

        this->owner->add_buff(buff);
    }

    return true;
}

// 受到伤害时
void EquipmentBase::on_being_hurt(int damage, int type, int sub_type)
{
    if (!equip_cfg) {
        return;
    }


}

void EquipmentBase::on_attack(float &damage)
{
    if (!equip_cfg) {
        return;
    }

    
}

void EquipmentBase::on_performed_skill()
{
    
}

void EquipmentBase::on_performing_skill(std::vector<BuffBase *> buffs)
{
    if (!this->equip_cfg) {
        return;
    }

    if (this->equip_cfg->skill_critical) {
        // TODO
    }
}
