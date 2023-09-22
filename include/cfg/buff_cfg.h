#ifndef __BUFF_CFG_H__
#define __BUFF_CFG_H__
#include "cfg.h"
#include <unordered_map>

struct Buff
{
    int id;                                                             // id
    char functype;                                                      // 作用类型
    char hurt_type;                                                     // 伤害类型
    char param_type;                                                    // 参数类型
    char property;                                                      // 属性类型，基础数值 or 百分比
    char trigger_cond;                                                  // 触发条件
    char spec_cond;                                                     // 特殊触发条件
    int trigger_time;                                                   // 触发次数
    float trigger_rate;                                                 // 触发概率
    bool die_keep;                                                      // 释放者死亡后，接收者是否保留
    int lasting;                                                        // 持续时间
    std::string impl_name;                                              // 实现类名
};

struct SimpleBufff
{
    int id;
    int buff_type;
    int buff_func_type;
    
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