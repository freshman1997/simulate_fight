#include "cfg/equip_cfg.h"
#include <stdlib.h>

bool EquipCfg::load()
{
    //READ_CFG("fight_misc.json", &EquipCfg::parse_equip_config)
    return true;
}

bool EquipCfg::parse_equip_config(json &)
{
    return true;
}

const Equip * EquipCfg::get_equip(int id)
{
    auto it = this->normal_equips.find(id);
    return it == this->normal_equips.end() ? nullptr : &it->second;
}

const Equip * EquipCfg::get_shard_equip(int id)
{
    auto it = this->shard_equips.find(id);
    return it == this->shard_equips.end() ? nullptr : &it->second;
}

static std::vector<int> lottery(const std::unordered_map<int, Equip> &equips, int amount)
{
    if (equips.empty() || amount <= 0 || amount >= equips.size()) {
        return {};
    }

    std::vector<int> res;
    for (int i = 0; i < amount; ++i) {
        res.push_back(rand() % equips.size());
    }

    return res;
}

std::vector<int> EquipCfg::lottery_shard_equip(int amount)
{
    return lottery(shard_equips, amount);
}

std::vector<int> EquipCfg::lottery_equip(int amount)
{
    return lottery(normal_equips, amount);
}