#ifndef __EVENT_H__
#define __EVENT_H__
#include <unordered_map>
#include <list>
#include <functional>

// 事件类型定义
enum class EventType : unsigned char
{
    UNIT_DIE,               // 死亡
    UNIT_ATTACK,            // 攻击
    UNIT_HURT,              // 受到伤害
    UNIT_ADD_BUFF,
    UNIT_RM_BUFF,
    
    FIGHT_START,            // 战斗开始
    FIGHT_END,              // 战斗结束
    GAME_START,             // 游戏开始
    GAME_END,               // 游戏结束
};

class FightUnit;

struct UnitDieParam
{
    FightUnit *die_unit = nullptr;
    FightUnit *killer = nullptr;
    bool atk = false;
    int actor_id = -1;
};

class EventParams
{
public:
    EventParams() : p1(0), p2(0), p3(0), p4(0), ptr(nullptr) {}
    EventParams(int _p1, int _p2 = 0, int _p3 = 0, int _p4 = 0) : p1(_p1), p2(_p2), p3(_p3), p4(_p4), ptr(nullptr) {}
    EventParams(void *p) : ptr(p) {}
    
public:
    int p1;
    int p2;
    int p3;
    int p4;
    void *ptr;
};

struct EventStruct
{
    int id;
    std::function<void (const EventParams &)> func = nullptr;
    void operator()(const EventParams &param) 
    {
        if (!func) {
            return;
        }

        func(param);
    }
};

class EventManager
{
public:
    EventManager();
    void register_event(EventType, int id, std::function<void (const EventParams &)>);
    void trigger_event(EventType, const EventParams &); 
    void remove_event(EventType, int id);

private:
    std::unordered_map<EventType, std::list<EventStruct>> functors;
};

#endif