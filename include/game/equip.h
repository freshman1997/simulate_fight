#ifndef __EQUIP_H__
#define __EQUIP_H__
#include "actor.h"
#include <vector>

class FightUnit;
struct Equip;
class BuffBase;

enum class equip_type : char
{
    none = 0,
    shard_equip,            // 散件
    normal_equip,           // 普通成装
    light_equip,            // 成装的光明版本
    auxlity_equip,          // 辅助装备
    aoen_equip,             // 奥恩神器
    fetter_equip,           // 羁绊装备
    other_equip,            // 其他装备
};

enum class other_equip_func_type
{
    none = 0,
    unequip,                // 卸下
    separate,               // 拆开
    re_rand,                // 重铸
    buff,                   // 给予一个 buff，比如祖安羁绊给到的装备
    copy,                   // 妮寇之助 复制
};

class EquipmentBase : public Actor
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();
    virtual void update(float deltaTime);

    // 进入战斗前计算
    virtual bool init();
    // 开始回合前
    virtual void on_before_round() {}   

    // 受到伤害时
    virtual void on_being_hurt(int damage, int type, int sub_type);
    virtual void on_attack(float &damage);
    virtual void on_performed_skill();
    virtual void on_performing_skill(std::vector<BuffBase *> buffs);
    
public:
    equip_type e_type = equip_type::none;
    const Equip *equip_cfg = nullptr;
    FightUnit *owner = nullptr;
    
    Vector2 pos = {0, 0};
    int extra_param1 = 0;
    int extra_param2 = 0;
    float extra_param3 = 0l;
};

#endif
