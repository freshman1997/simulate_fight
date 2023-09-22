#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stdint.h>
#include <unordered_map>
#include <vector>

#include "actor.h"

class Fight;
class EquipmentBase;
class EventManager;
class FightUnit;
class Game;
class BuffBase;
class FetterBase;

struct EquipHolder
{
    Vector2 pos;
    EquipmentBase *equip;
};

// 单个玩家
class GamePlayer
{
public:
    void update(float delta);

    inline void on_fight_begin();

    // 战斗结束调用
    void on_fight_end(bool win, int rest);

    void add_unit_buff(BuffBase *);

    // 羁绊、海克斯变化后重新计算buff等信息
    void on_changed();

public:
    // 使用金币
    bool use_gold(int amount);

    // 抽卡
    void lottery_hero();

    // 选择卡牌
    void select_hero(int idx);

    // 升级
    void upgrade();

    // 上场英雄
    void push_hero(int , Vector2 pos);

    // 卖掉英雄
    void sell_hero(bool board, int id);

    // 替换英雄
    void replace_hero(int id1, int id2);

    // 放置装备
    void puton_equip(bool board, int id1, int id2);

    // 选择海克斯buff
    void select_buff(int idx);

    // 获取战斗相关的buff
    std::vector<BuffBase *> get_fight_buffs();

public:
    int player_id = -1;                                              // 玩家id
    int hp = 100;                                                    // 当前血量
    int gold = 0;                                                    // 金币数量
    int level;                                                       // 当前等级
    int win_amount = 0;                                              // 当前胜利场数 
    int defeat_amount = 0;                                           // 当前被打败场数
    int speed = 0;                                                   // 小小英雄移速
    int interest = 0;                                                // 当前利息，海克斯可以修改
    int flush_time = 0;                                              // 抽卡次数
    Vector2 pos = {0, 0};                                       // 当前所在位置
    std::unordered_map<int, EquipHolder> equip_holder;               // 装备
    std::unordered_map<int, FightUnit *> boardHeros;                 // 棋盘上的棋子
    std::unordered_map<int, FightUnit *> holderHeros;                // 备战区的棋子
    std::vector<int> candidate_buffs;                                // 本次候选的海克斯buff
    std::vector<BuffBase *> fight_buffs;                             // 海克斯产生的战斗buff
    std::vector<std::pair<int, bool>> lottery_candidate_heros;       // 刷新的卡牌
    Game *own_game = nullptr;                                        // 所属场次对象
    Fight *fight_obj = nullptr;                                      // 所属回合对象
    std::vector<FetterBase *> fetters;                               // 激活的羁绊列表

    std::vector<BuffBase *> cache_buffs;                             // 暂时缓存的buff
};

#endif