#ifndef __HERO_H__
#define __HERO_H__
#include <unordered_map>
#include <vector>
#include <string>

#include "actor.h"
#include "game/actor.h"
// 战斗怎么做？
/*
    1、先寻找目标
    2、检查攻击距离，不够则寻路过去
    3、普攻攻击并产生蓝量、蓝量满了释放技能
    4、目标死亡切换回到 1
    5、如果没办法攻击，如被占满外围位置了，则不动等待其他棋子攻击
*/

class AiBase;
class SkillBase;
class EquipBase;

#define add_xx(xx) void add_##xx(int num) { this->xx += num; }

class FightUnit : public Actor
{
public:
    add_xx(hp)
    add_xx(mp)
    add_xx(atk_val)
    add_xx(ap_val)
    add_xx(atk_speed)
    add_xx(ad_def_val)
    add_xx(ap_def_val)
    add_xx(critical_rate)
    add_xx(critical_extra)
    add_xx(atk_distance)
    add_xx(move_speed)

public:
    int side;                   // 用于战斗期间
    int rest_time;              // 上一帧不够动作剩余时间

public:
    int hp = 0;                 // 血量
    int mp = 0;                 // 蓝量
    int atk_val = 0;            // 攻击力
    int ap_val = 0;             // 法强
    int atk_speed = 0;          // 攻速
    int ad_def_val = 0;         // 护甲
    int ap_def_val = 0;         // 魔抗
    int critical_rate = 0;      // 暴击率
    int critical_extra = 0;     // 暴击加成
    int atk_distance = 0;       // 攻击距离
    int move_speed = 0;         // 移速

public:
    // position
    Vector2 pos;
    
public:
    const std::vector<std::pair<int, int>> *path = nullptr;   // 暂存的路径
    FightUnit *enemy = nullptr;                               // 暂存的目标  

public:
    // 技能、AI
    AiBase *ai;
    SkillBase *skill;

    // 装备列表，包括散件和成装
    std::vector<EquipBase *> equipments;
};

// 英雄
class HeroBase : public FightUnit
{

public:
    virtual void update();

public:
    int id;

private:
    bool hasAngue;
    int angue;
    int blood;
    int star;
    std::unordered_map<std::string, int> extraData;
};

#endif