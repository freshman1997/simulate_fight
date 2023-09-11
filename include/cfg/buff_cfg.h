#ifndef __BUFF_CFG_H__
#define __BUFF_CFG_H__
#include "cfg.h"
#include "game/buff.h"
#include <unordered_map>

struct Buff
{
    int id;
    buff_func_type functype;
    buff_hurt_type hurt_type;
    buff_param_type param_type;
    buff_property_type property;
    buff_trigger_condition trigger_cond;
    buff_specical_trigger_cond spec_cond;
    int trigger_time;
    float trigger_rate;
    bool die_keep;
    int target_amount;
    int lasting;
    std::unordered_map<std::string, float> properties;
    std::string impl_name;
};

class BuffCfg : public CfgBase
{
public:
    virtual bool load();

public:
    bool parse_buff_cfg(json &);
    const Buff * get_buff(int id);

private:
    std::unordered_map<int, Buff> buffs;
};

#endif