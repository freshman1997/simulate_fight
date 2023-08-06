#include "game/event/event.h"

EventManager & EventManager::get_instance()
{
    static EventManager mgr;
    return mgr;
}

void EventManager::register_event(EventType type, std::function<void (EventParams)> func)
{
    auto &list = this->functors[type];
    list.push_back(func);
}

void EventManager::on_trigger_event(EventType type, const EventParams &params)
{
    auto it = this->functors.find(type);
    if (it == this->functors.end()) {
        return;
    }

    for (auto &func : it->second) {
        func(params);
    }
}
