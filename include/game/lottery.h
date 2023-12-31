﻿#ifndef __LOTTERY_H__
#define __LOTTERY_H__
#include <vector>
#include <unordered_map>

class Equip;

class LotteryHelper
{
public:
    LotteryHelper();

    void init_pool();

public:
    // 随机阵营
    std::vector<int> lottery_scene(int amount);

    // 抽卡
    void lottery_heros(int lv, int amount, std::vector<int> &res);

    // 抽装备
    inline std::vector<int> lottery_equips(int amount);

    // 选秀, 下标为第几个，<id, 装备id列表>
    std::vector<std::pair<int, std::vector<int>>> lottery_xuanxiu(int amount, int equips, int extra_equips, int phase);

    // 海克斯潘多拉备战席
    std::vector<int> lottery_candidate_hero(const std::vector<int> &heros);

    // 海克斯，潘多拉装备
    std::vector<int> lottery_equip();

    // 随机一个
    int rand_one();

    // 随机两个
    std::pair<int, int> rand_two();

    // 是否可以
    bool rand_can_do(int total, int rate);

    // 随机一个范围内的数值
    int rand_num(int max_amount);

    // 是否命中
    bool rand_one(float ratio);
public:
    // 随机装备（成装）
    std::vector<const Equip *> rand_equips(int amount, int type);

public:
    // 卖掉卡
    void release(int hero_id, int star);

public:
    // 下标是卡费，值是卡牌对应剩余数量
    std::vector<std::unordered_map<int, int>>  pool;
};


#endif