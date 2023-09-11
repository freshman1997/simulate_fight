#ifndef __BUFF_H__
#define __BUFF_H__
#include "actor.h"
#include <vector>

class FightUnit;
struct Buff;

enum class buff_func_type : char
{
    none = -1,
    hp,                     // 血量
    mp,                     // 蓝量
    atk_def,                // 护甲
    ap_def,                 // 魔抗
    atk,                    // 攻击力
    atk_dis,                // 攻击距离
    ap_val,                 // 法强
    atk_sp,                 // 攻速
    shield,                 // 护盾
    silence,                // 沉默
    stun,                   // 眩晕
    imprison,               // 禁锢
    strike_of_fly,          // 击飞
    disarm,                 // 缴械
    taunt,                  // 嘲讽
};

enum class buff_param_type : char
{
    none = -1,
    basic,                  // 基本属性
    percent,                // 属性百分比
};

enum class buff_property_type : char
{
    none = -1,
    hp,
    mp,
    atk_val,
    ap_val,
    atk_speed,
    ad_def_val,
    ap_def_val,
    critical_rate,
    critical_extra,
    atk_distance,
    move_speed,
};

enum class buff_hurt_type : char
{
    none = -1,
    physics,
    magic,
    real,
};

enum class buff_trigger_condition : char
{
    none = -1,
    hurt,                   // 受到伤害时
    stuck,                  // 被控制时 
    walk,                   // 移动时
    being_atk,              // 被普通攻击时
    cure,                   // 回血时
    mp,                     // 回蓝时
    atk,                    // 普攻时
    kill,                   // 击杀英雄时
    unit_kill,              // 任意友方单位击杀时
};

enum class buff_specical_trigger_cond : char 
{
    none = -1,
    inc_hurt,               // 增伤
    dec_hurt,               // 减伤
    behead,                 // 斩杀
};

class BuffBase : public Actor
{
public:
    virtual void update(float deltaTime);
    virtual Object * clone();
    virtual Object * clone_and_clean();

    virtual void on_add();
    virtual void on_remove();
    void reset();
    void free();

public:
    std::vector<float> params;
    const Buff *buff_cfg = nullptr;
    FightUnit *from = nullptr;
    FightUnit *to = nullptr;
    int trigger_time = 0;
    float buff_time = 0;
};

#endif
