#ifndef __PLAYER_CFG_H__
#define __PLAYER_CFG_H__

#include "cfg/cfg.h"
#include <vector>

struct player_misc
{
    int gold_2_exp[2];                                  // 金币和经验的比例
    int gold_interest[2];                               // 金币和利息的比例
    int max_interest;                                   // 默认最大利息数
    int min_exp;                                        // 单次购买经验最小数量
    int refresh_amount;                                 // 刷新一次的卡牌数量
    int refresh_cost;                                   // 刷新消耗金币数量
    int max_candidate_hero;                             // 备战区最大数量
    int win_gold;                                       // 胜利后可以获得金币数量
    std::vector<int> win_golds;                         // 战胜获得的奖金
    std::vector<int> defeat_golds;                      // 战败获得的奖金
    std::vector<int> phase_basic_survive_damage;        // 基础伤害
    std::vector<int> phase_survive_damage;              // 根据阶段存活单位数量造成的伤害
};

class PlayerCfg : public CfgBase
{
public:
    virtual bool load();

    bool parse(json &);

    player_misc misc;
};

#endif
