#ifndef __LOTTERY_CFG_H__
#define __LOTTERY_CFG_H__
#include <unordered_map>
#include <vector>
#include "cfg.h"

struct lottery_ratio
{
    int phase;
    int single_amount;
    std::vector<float> lottery_ratio;
};

struct star_cost
{
    int gold;
    int up_star[3];
    int sell[3];
};

struct card_pool
{
    int lv;
    int per;
    std::vector<int> candidate_list;
};

class LotteryCfg : public CfgBase
{
public:
    virtual bool load();

    const star_cost * get_star_cost(int hero_gold);
    const lottery_ratio * get_lottery_ratio(int lv);
    std::vector<card_pool *> get_level_pool(int lv);

private:

};


#endif // !__LOTTERY_CFG_H__