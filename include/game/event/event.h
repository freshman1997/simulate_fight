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
    
};

class EventParams
{
public:
    EventParams() : p1(0), p2(0), p3(0), p4(0) {}
    EventParams(int _p1, int _p2 = 0, int _p3 = 0, int _p4 = 0) : p1(_p1), p2(_p2), p3(_p3), p4(_p4) {}

public:
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
};

class EventManager
{
public:
    EventManager();
    void register_event(EventType, std::function<void (const EventParams &)>);
    void trigger_event(EventType, const EventParams &); 

private:
    std::unordered_map<EventType, std::list<std::function<void (const EventParams &)>>> functors;
};

#endif