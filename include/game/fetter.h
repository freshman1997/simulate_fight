#ifndef __FETTER_H__
#define __FETTER_H__
// 羁绊
#include "actor.h"

class GamePlayer;
class Fetter;

// 云顶之奕 S9.5 羁绊
enum class fetter_type
{
    wandering_mage = 1,                     // 流浪法师
    dark_descendant,                        // 暗裔
    demasia,                                // 德玛西亚
    freildred,                              // 弗雷尔卓德
    eurasia,                                // 亚欧尼亚
    knoxus,                                 // 诺克萨斯
    piltwolf,                               // 皮尔特沃夫
    shurima,                                // 恕瑞玛
    colosseum,                              // 巨神峰
    emptiness,                              // 虚空
    zauin,                                  // 祖安
    bill_gewert,                            // 比尔吉沃特
    lsutar,                                 // 以绪塔尔
    fortress_guard,                         // 堡垒卫士
    gladiator,                              // 格斗家
    challenger,                             // 挑战者
    queen,                                  // 女皇
    gunner,                                 // 枪手
    oracle,                                 // 神谕者
    dominator,                              // 主宰
    mage,                                   // 术士
    stalker,                                // 潜行者
    judge_warrior,                          // 裁决战士
    wizard,                                 // 法师
    commander,                              // 司令
    great_inventor,                         // 大发明家
    ocean_disaster,                         // 海洋之灾
    conqueror,                              // 征服者
};

class FetterBase : public Actor
{
public:
    // 计算buff等
    virtual void on_begin();
    // 正式开启战斗之前
    virtual void on_before_round() {}

public:
    int active = -1;                    // 当前激活的层级
    Fetter *fetter_cfg = nullptr;
    GamePlayer *player = nullptr;
};

#endif
