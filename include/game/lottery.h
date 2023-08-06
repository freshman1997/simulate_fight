#ifndef __LOTTERY_H__
#define __LOTTERY_H__
#include <vector>
#include <unordered_map>

class LotteryHelper
{
public:
    // 单词花钱刷新
    std::vector<int> lottery(int lv);

    // 选秀, 下标为第几个，<id, 装备id列表>
    std::vector<std::pair<int, std::vector<int>>> lottery_xuanxiu(int amount, int equips, int phase);

    // 海克斯潘多拉备战席
    std::vector<int> lottery_3();

    // 海克斯，潘多拉装备
    std::vector<int> lottery_equip();

public:
    // 合成卡
    void compound();
    // 卖掉卡
    void release();

public:
    // 下标是卡费，值是剩余数量
    std::vector<int>  pool;
};


#endif