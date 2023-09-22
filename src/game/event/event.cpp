#include "game/event/event.h"

EventManager::EventManager()
{
    this->functors.clear();
}

void EventManager::register_event(EventType type, int id, std::function<void (const EventParams &)> func)
{
    this->functors[type].push_back({id, func});
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

void EventManager::remove_event(EventType type, int id)
{
    auto it = this->functors.find(type);
    if (it == this->functors.end()) {
        return;
    }

    for (auto ev_it = it->second.begin(); ev_it != it->second.end(); ++ev_it) {
        if (ev_it->id == id) {
            it->second.erase(ev_it);
            break;
        }
    }
}
