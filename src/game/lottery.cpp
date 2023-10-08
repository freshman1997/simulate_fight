#include <cmath>
#include <iterator>
#include <stdlib.h>
#include <utility>
#include <vector>
#include "cfg/equip_cfg.h"
#include "cfg/game_cfg.h"
#include "cfg/hero_cfg.h"
#include "cfg/lottery_cfg.h"
#include "game/equip.h"
#include "game/lottery.h"
#include "manager/cfg_manager.h"

LotteryHelper::LotteryHelper()
{
    init_pool();
}

void LotteryHelper::init_pool()
{
    // 这里应该去遍历英雄表
    CfgManager::get_instance().hero_cfg.for_each_heros([this](const Hero &hero) -> bool {
        if (pool.size() < hero.gold) {
            pool.push_back({});
        }

        pool[hero.gold - 1][hero.hero_id] = hero.card_amount;
        return true;
    });
}

// 随机阵营
std::vector<int> LotteryHelper::lottery_scene(int amount)
{
    std::vector<int> res;

    

    return res;
}


// 单次花钱刷新
void LotteryHelper::lottery_heros(int lv, int amount, std::vector<int> &res)
{
    const lottery_ratio *ratio = CfgManager::get_instance().lottery_cfg.get_lottery_ratio(lv);
    if (!ratio) {
        return;
    }

    float total_ratio = 0;
    for (auto &it : ratio->lottery_ratio) {
        total_ratio += it;
    }

    for (int i = 0; i < amount; ++i) {
        int rand_num = rand() % (int)std::ceil(total_ratio) + 1;
        int j = 0;
        for (; j < ratio->lottery_ratio.size(); ++j) {
            rand_num -= ratio->lottery_ratio[j];
            if (rand_num <= 0) {
                break;
            }
        }

        std::unordered_map<int, int> &pool_items = pool[j];
        int rand_idx = rand() % pool_items.size();
        auto it = pool_items.begin();

        for (int k = 0; k < rand_idx; ++k, ++it);
        int loop_cnt = 0;
        while (loop_cnt < pool_items.size()) {
            if (it->second > 0) {
                break;
            } else {
                ++it;
                if (it == pool_items.end()) {
                    it = pool_items.begin();
                }
            }

            ++loop_cnt;
        }

        if (it->second <= 0) {
            return;
        }

        res.push_back(it->first);
        --it->second;
    }  
}

std::vector<int> LotteryHelper::lottery_equips(int amount)
{
    return CfgManager::get_instance().equip_cfg.lottery_shard_equip(amount);
}

std::vector<std::pair<int, std::vector<int>>> LotteryHelper::lottery_xuanxiu(int amount, int equips, int extra_equips, int phase)
{
    std::vector<std::pair<int, std::vector<int>>> res;

    const game_phase *phase_cfg = CfgManager::get_instance().game_cfg.get_phase(phase);
    if (!phase_cfg) {
        return res;
    }
    
    int real_equips = equips * (extra_equips > 0 ? extra_equips : 1);

    std::vector<int> res_heros;
    lottery_heros(phase_cfg->amount, real_equips, res_heros);
    const auto &res_equips = lottery_equips(real_equips);
    
    if (res_heros.empty() || res_equips.empty()) {
        return res;
    }

    int equip_idx = 0;

    for (int i = 0; i < amount; ++i) {
        std::pair<int, std::vector<int>> p;

        if (extra_equips > 0) {
            for (int j = 0; j < extra_equips; ++i, ++equip_idx) {
                p.second.push_back(res_equips[equip_idx]);
            }
        } else {
            p.second.push_back(res_equips[equip_idx]);
            ++equip_idx;
        }

        p.first = res_heros[i];
        res.push_back(p);
    }

    return res;
}

// 是否可以
bool LotteryHelper::rand_can_do(int total, int rate)
{
    int num = rand() % total + 1;
    return num <= rate;
}

// 随机一个范围内的数值
int LotteryHelper::rand_num(int max_amount)
{
    return rand() % max_amount;
}

// 是否命中
bool LotteryHelper::rand_one(float ratio)
{
    if (ratio <= 0 ) return false;

    return rand() % 10000 <= ratio * 10000;
}


void LotteryHelper::release(int hero_id, int star)
{
    const Hero *hero = CfgManager::get_instance().hero_cfg.get_hero(hero_id);
    if (!hero || (star <= 0 || star > 3)) return;

    int amount = star == 1 ? 1 : (star == 2 ? 2 : 3);
    this->pool[hero->gold][hero_id] += amount;
}

////////////////// equip /////////////////////
std::vector<const Equip *> LotteryHelper::rand_equips(int amount, int type)
{
    std::vector<const Equip *> res;

    if (equip_type(type) == equip_type::normal_equip) {
        const std::vector<int> &rand_equip =  lottery_equips(amount);
        for (auto &it : rand_equip) {
            res.push_back(CfgManager::get_instance().equip_cfg.get_equip(it));
        }
    } else if (equip_type(type) == equip_type::shard_equip) {
        const std::vector<int> &rand_equip =  CfgManager::get_instance().equip_cfg.lottery_shard_equip(amount);
        for (auto &it : rand_equip) {
            res.push_back(CfgManager::get_instance().equip_cfg.get_equip(it));
        }
    }

    return res;
}
