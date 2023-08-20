#ifndef __HERO_CFG_H__
#define __HERO_CFG_H__
#include "cfg.h"

struct Hero
{
    int hero_id;                                                // id
    int hp[3];                                                  // 初始血量
    int mp[3];                                                  // 星级对应最大蓝量
    int init_mp[3];                                             // 初始蓝量
    float atk_speed[3];                                         // 初始攻速
    int atk_val[3];                                             // 初始攻击力
    int atk_def[3];                                             // 初始护甲
    int ap_def[3];                                              // 初始魔抗
    int critical_rate[3];                                       // 初始暴击几率
    int critical_tims[3];                                       // 初始暴击伤害倍率
    int move_speed[3];                                          // 初始移速
    int atk_distance[3];                                        // 星级对应的初始攻击距离
    int skill_id;                                               // 技能id
    int max_star;                                               // 最大星级
    int gold;                                                   // 费率
    std::unordered_map<int, std::pair<int, int>> merge;         // 合成高一费卡所需，2 -> 1, 3
    std::string desc;
    std::vector<int> careers;                                   // 羁绊/职业列表
    std::string name;
};

class HeroCfg : public CfgBase
{
public:
    virtual bool load();

    bool parse_hero_cfg(json &);

    const Hero * get_hero(int id);
    
private:
    std::unordered_map<int, Hero> heros;
};

#endif