#ifndef  __EQUIP_CFG_H__
#define __EQUIP_CFG_H__
#include <unordered_map>
#include <vector>

#include "cfg/cfg.h"

struct equip
{
    int id;
    bool is_shard;      // 是否为散件

};

class EquipCfg : public CfgBase
{
public:
    virtual bool load();

public:
    bool parse_equip_config(json &);

    const equip * get_equip(int id);

    const equip * get_shard_equip(int id);

    std::vector<int> lottery_shard_equip(int amount);

    std::vector<int> lottery_equip(int amount);

private:
    std::unordered_map<int, equip> equips;
    std::unordered_map<int, equip> shard_equips;
};

#endif // ! __EQUIP_CFG_H__
