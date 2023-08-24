#include "game/event/event.h"

EventManager::EventManager()
{
    this->functors.clear();
}

void EventManager::register_event(EventType type, std::function<void (const EventParams &)> func)
{
    auto &list = this->functors[type];
    list.push_back(func);
}

void EventManager::trigger_event(EventType type, const EventParams &params)
{
    auto it = this->functors.find(type);
    if (it == this->functors.end()) {
        return;
    }

    for (auto &func : it->second) {
        func(params);
    }
}
