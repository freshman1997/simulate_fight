#ifndef __HERO_H__
#define __HERO_H__
#include <unordered_map>
#include <vector>
#include <string>

#include "actor.h"
#include "game/actor.h"
#include "cfg/hero_cfg.h"
#include "buff.h"
#include "event/event.h"

class AiBase;
class SkillBase;
class EquipBase;
class Fight;

#define add_xx(xx, _type) void add_##xx(_type num) \
{ \
    this->xx = this->on_value_change<_type>(this->xx, num, ListenType::xx); \
\
}

enum class ListenType
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
    stuck,
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
    virtual void init() = 0;
    virtual void after_init_check() = 0;

public:
    virtual void attack() = 0;
    virtual void move() = 0;
    virtual void on_being_attack() = 0;
    virtual void buff_action() = 0; 
    virtual void on_buff_end(BuffBase *) = 0;
    virtual void on_skill_end() { mp = 0; }
    virtual bool can_perform_skill() { return skill != nullptr; }
    virtual bool can_attack() 
    { 
        // TODO
        return stuck; 
    }
    
public:
    bool is_die() { return hp <= 0; }

    void add_buff(BuffBase *buf) 
    { 
        if (!buf || is_die()) return;

        auto it = buffs.find(buf->id);
        if (it == buffs.end()) {
            buffs[buf->id] = buf; 
        }
    }

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
    add_xx(stuck, bool)

    template<class T>
    void register_handler(T *handler)
    {
        static_assert(std::is_base_of<ValueHandler, T>::value, "T must base of ValueHandler");
        if (!handler || handler->listen == ListenType::none) return;

        handlers[handler->listen].push_back(handler);
    }

    template<class T>
    T on_value_change(T base, T add, ListenType type)
    {
        Value val;
        if (std::is_same<int, T>::value) {
            val.type = ValueType::int_val;
            val.base_val.ival = int(base);
            val.change_val.ival = int(add);
        } else if (std::is_same<float, T>::value) {
            val.type = ValueType::float_val;
            val.base_val.fval = float(base);
            val.change_val.fval = float(add);
        } else if (std::is_same<double, T>::value) {
            val.type = ValueType::double_val;
            val.base_val.dval = double(base);
            val.change_val.dval = double(add);
        } else if (std::is_same<bool, T>::value) {
            val.type = ValueType::bool_val;
            val.base_val.bval = base;
            val.change_val.bval = add;
        }

        auto it = handlers.find(type);
        if (it == handlers.end()) {
            if (std::is_same<bool, T>::value) {
                return add;
            }

            return base + add;
        }

        for (auto h = it->second.begin(); h != it->second.end();) {
            (*h)->on_value_changed(val);
            if ((*h)->remove()) {
                h = it->second.erase(h);
            } else {
                ++h;
            }
        }

        T res = std::move(val.get_value<T>());

        post_process<T>(res, type);

        return res;
    }

    template<class T>
    void post_process(T &res, ListenType type)
    {
        switch (type)
        {
        case ListenType::hp: {
            if (int(res) <= 0) {
                trigger_event(EventType::UNIT_DIE, {});
            }
            break;
        }
        
        default:
            break;
        }
    }

public:
    void trigger_event(EventType type, const EventParams &);
    void register_event(EventType, std::function<void (const EventParams &)>);

public:
    int player_id = 0;
    int side = 0;               // 用于战斗期间
    float rest_time = 0;        // 上一帧不够动作剩余时间

public:
    int hp = 0;                 // 血量
    int mp = 0;                 // 蓝量
    int atk_val = 0;            // 攻击力
    int ap_val = 0;             // 法强
    float atk_speed = 0;        // 攻速
    int ad_def_val = 0;         // 护甲
    int ap_def_val = 0;         // 魔抗
    int critical_rate = 0;      // 暴击率
    int critical_extra = 0;     // 暴击加成
    int atk_distance = 0;       // 攻击距离
    int move_speed = 0;         // 移速

public:
    int shield = 0;             // 护盾值

public:
    // position
    Vector2 pos;
    Fight *round_obj = nullptr;
    const Hero *hero_cfg = nullptr;
    char star = 0;
    bool stuck = false;

public:
    const std::vector<std::pair<int, int>> *path = nullptr;   // 暂存的路径
    FightUnit *enemy = nullptr;                               // 暂存的目标  

public:
    // 技能、AI
    AiBase *ai = nullptr;
    SkillBase *skill = nullptr;

    // 装备列表，包括散件和成装
    std::unordered_map<int, EquipBase *> equipments;
    std::unordered_map<int, BuffBase *> buffs;

private:
    std::unordered_map<ListenType, std::list<ValueHandler *>> handlers;
};

// 英雄
class HeroBase : public FightUnit
{

public:
    HeroBase();
    virtual Object * clone();
    virtual Object * clone_and_clean();

    virtual void init();
    virtual void update(float deltaTime);

    virtual void after_init_check(){};

    virtual void move();
    virtual void attack();
    virtual void on_being_attack();
    virtual void buff_action();
    virtual void on_buff_end(BuffBase *);

private:
    void set_properties(HeroBase *);
    void do_attack();

protected:
    float calc_atk_speed();
    float delta;
    float cumulative_atk_time;
    float atk_time;
};

#endif