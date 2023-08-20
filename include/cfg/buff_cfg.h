#ifndef __BUFF_CFG_H__
#define __BUFF_CFG_H__
#include <vector>
#include "cfg.h"
#include "game/buff.h"

struct Buff
{
    int id;
    buff_type type;
    buff_sub_type subtype;
    buff_func_type functype;
    buff_trigger_condition trigger_cond;
    int trigger_time;
    float trigger_rate;
    bool die_keep;
    buff_target_choose_rule choose_rule;
    int target_amount;
    buff_target_sort_type sort_type;
    int lasting;
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