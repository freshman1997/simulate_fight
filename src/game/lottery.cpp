#include <stdlib.h>
#include "game/lottery.h"

std::vector<std::pair<int, std::vector<int>>> LotteryHelper::lottery_xuanxiu(int amount, int equips, int phase)
{
    std::vector<std::pair<int, std::vector<int>>> res;

    int hsz = 50;
    int esz = 50;

    for (int i = 0; i < amount; i++) {
        int hid = rand() % hsz;
        res.push_back({hid, {}});
        for (int j = 0; j < equips; ++j) {
            res.back().second.push_back(rand() % esz);
        }
    }

    return res;
}
