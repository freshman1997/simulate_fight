#ifndef __HERO_H__
#define __HERO_H__
#include <unordered_map>
#include <vector>
#include <string>
#include <list>

#include "actor.h"
#include "game/actor.h"
#include "cfg/hero_cfg.h"
#include "buff.h"
#include "event/event.h"

class AiBase;
class SkillBase;
class EquipmentBase;
class Fight;
class GamePlayer;

#define add_xx(xx, _type) void add_##xx(_type num) \
{ \
    this->xx += num; \
} \
_type max_##xx = 0;


#define add_xx_1(xx, _type) void add_##xx(_type num) \
{ \
    this->xx = num; \
} \
_type max##xx = 0;

enum class hurt_t
{
    attack,
    buff,
    skill,
};

enum class hurt_sub_t
{
    physics,        // 普通物理伤害
    magic,          // 普通魔法伤害
    real,           // 真实伤害
};

class FightUnit : public Actor
{
protected:
    enum class attack_state
    {
        prepare,        // 未就绪
        before,         // 前摇
        after           // 后摇
    };
    attack_state atk_state = attack_state::prepare;

public:
    virtual void after_init_check() = 0;

public:
    virtual void attack() = 0;

    virtual void on_target_moved() = 0;
    virtual void on_being_attack(FightUnit *from, float damage) = 0;
    virtual void buff_action() = 0; 
    virtual void on_skill_end() { mp = 0; }

    virtual bool can_move();
    virtual bool can_perform_skill();
    virtual bool can_attack();
        
    virtual void perform_hurt(hurt_t, hurt_sub_t, FightUnit *from, FightUnit *to, float damage, int actor_id) = 0;
    
public:
    bool is_die() { return hp <= 0; }

    void add_buff(BuffBase *buf) 
    { 
        if (!buf || is_die()) return;

        do_add_buff(buf);
    }

    void do_add_buff(BuffBase *buf);
    void trigger_buff(BuffBase *buf);
    void on_buff_end(BuffBase *);
    void set_stuck(bool);
    void clear_stuck();

public:
    void on_attack(float &damage);
    void on_performed_skill();

public:
    add_xx(hp, int)
    add_xx(mp, int)
    add_xx(atk_val, int)
    add_xx(ap_val, int)
    add_xx(atk_speed, float)
    add_xx(ad_def_val, int)
    add_xx(ap_def_val, int)
    add_xx(critical_rate, int)
    add_xx(critical_extra, int)
    add_xx(atk_distance, int)
    add_xx(move_speed, int)
    add_xx(shield, int)
    add_xx_1(stuck, bool)

public:
    void trigger_event(EventType type, const EventParams &);
    void register_event(EventType, int id, std::function<void (const EventParams &)>);
    void remove_event(EventType, int id);

public:
    int side = 0;               // 用于战斗期间
    bool skill_critic = false;  // 技能是否可以暴击

public:
    int hp = 0;                 // 血量
    int mp = 0;                 // 蓝量
    int atk_val = 0;            // 攻击力
    int ap_val = 0;             // 法强
    float atk_speed = 0;        // 攻速
    float extra_atk_speed = 0;  // 额外攻速
    int ad_def_val = 0;         // 护甲
    int ap_def_val = 0;         // 魔抗
    int critical_rate = 0;      // 暴击率
    int critical_extra = 0;     // 暴击加成
    int atk_distance = 0;       // 攻击距离
    int move_speed = 0;         // 移速
    int shield = 0;             // 护盾值

    int cover_size[2] = {0};

public:
    bool is_sommon = false;

public:
    // position
    Vector2 pos;
    float move_distance = 0l;
    int move_step = -1;
    char star = 0;
    bool stuck = false;
    bool is_imprison = false;
    
public:
    Fight *round_obj = nullptr;
    const Hero *hero_cfg = nullptr;

public:
    std::vector<Vector2> path;                     // 暂存的路径
    FightUnit *enemy = nullptr;                               // 暂存的目标  
    GamePlayer *owner = nullptr;                              // 暂存的玩家对象  

public:
    // 技能、AI
    AiBase *ai = nullptr;
    SkillBase *skill = nullptr;
 
    // 装备列表，包括散件和成装
    std::unordered_map<int, EquipmentBase *> equipments;
    std::unordered_map<int, BuffBase *> buffs;
    std::unordered_map<buff_trigger_condition, std::unordered_map<int, BuffBase *>> trigger_buffs;
};

// 英雄
class HeroBase : public FightUnit
{

public:
    HeroBase();
    virtual Object * clone();
    virtual Object * clone_and_clean();
    
    virtual bool init();
    virtual void update(float deltaTime);

    // 造成伤害的统一接口
    virtual void perform_hurt(hurt_t, hurt_sub_t, FightUnit *from, FightUnit *to, float damage, int actor_id);

    virtual void after_init_check(){};

    virtual void on_target_moved();
    virtual void attack();
    virtual void on_being_attack(FightUnit *from, float damage);
    virtual void buff_action();

    float calc_damage(hurt_sub_t type, float damage);

private:
    void set_properties(HeroBase *);
    void do_attack();
    void perform_hurt_mp(int damage);

protected:
    float calc_atk_speed();
    float delta;
    float cumulative_atk_time;
    float atk_time;
    float walk_start_time;
};

#endif