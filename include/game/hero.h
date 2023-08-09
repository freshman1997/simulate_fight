#ifndef __HERO_H__
#define __HERO_H__
#include <unordered_map>
#include <vector>
#include <string>

#include "actor.h"
#include "game/actor.h"

class AiBase;
class SkillBase;
class EquipBase;

enum class FightState
{
    moving,
    attacking,
    performing_skill,
    stuck,

};

#define add_xx(xx) void add_##xx(int num) { this->xx += num; }

class FightUnit : public Actor
{
public:
    virtual void init() = 0;
    virtual void after_init_check() = 0;
    virtual float get_cumulative_time() = 0;

public:
    virtual void attack() = 0;
    

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
    int side = 0;               // 用于战斗期间
    float rest_time = 0;        // 上一帧不够动作剩余时间

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
    AiBase *ai = nullptr;
    SkillBase *skill = nullptr;

    // 装备列表，包括散件和成装
    std::vector<EquipBase *> equipments;
};

// 英雄
class HeroBase : public FightUnit
{

public:
    HeroBase();
    virtual void init();
    virtual void update();

public:
    
};

#endif