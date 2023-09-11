#include "cfg/lottery_cfg.h"


bool LotteryCfg::load()
{
    return true;
}

const star_cost * LotteryCfg::get_star_cost(int hero_gold)
{
    return nullptr;
}

const lottery_ratio * LotteryCfg::get_lottery_ratio(int lv)
{
    return nullptr;
}

std::vector<card_pool *> LotteryCfg::get_level_pool(int lv)
{
    return {};
}