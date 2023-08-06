#ifndef __EVENT_H__
#define __EVENT_H__
#include <unordered_map>
#include <list>
#include <functional>

// 事件类型定义
enum class EventType : unsigned char
{
    HERO_HP_DEC,
    HERO_DIE,
    
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
    static EventManager & get_instance();
    void register_event(EventType, std::function<void (EventParams)>);
    void on_trigger_event(EventType, const EventParams &); 

private:
    std::unordered_map<EventType, std::list<std::function<void (const EventParams &)>>> functors;
};

#define REGISTER_EVENT(type, func) EventManager::get_instance().register_event(type, func)

#endif