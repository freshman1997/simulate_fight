#ifndef  __EQUIP_CFG_H__
#define __EQUIP_CFG_H__
#include <unordered_map>
#include <utility>
#include <vector>

#include "cfg/cfg.h"
#include "nlohmann/json_fwd.hpp"

struct Equip
{
    int id;
    int equip_type;                                                     // 装备类型
    bool is_shard;                                                      // 是否为散件
    bool skill_critical;                                                // 技能是否可以暴击
    bool unique;                                                        // 是否唯一
    std::vector<std::pair<int, std::vector<float>>> buffs;              // buff 列表
    std::vector<std::pair<int, std::vector<float>>> light_buffs;        // 光明 buff 列表
    json extra_params;                                                  // 额外参数
    std::string impl_name;
};

class EquipCfg : public CfgBase
{
public:
    virtual bool load();

public:
    bool parse_equip_config(json &);

    const Equip * get_equip(int id);

    const Equip * get_shard_equip(int id);

    std::vector<int> lottery_shard_equip(int amount);

    std::vector<int> lottery_equip(int amount);

private:
    std::unordered_map<int, Equip> normal_equips;
    std::unordered_map<int, Equip> shard_equips;
};

#endif // ! __EQUIP_CFG_H__
