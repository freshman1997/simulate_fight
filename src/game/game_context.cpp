#include "game/game_context.h"
#include "game/actor.h"

GameContext & GameContext::get_game_context()
{
    static GameContext ctx;
    return ctx;
}

FightContext * GameContext::find_context(std::thread::id _id)
{
    auto it = this->contexts.find(_id);
    if (it == this->contexts.end()) {
        return nullptr;
    }

    return &it->second;
}

void GameContext::remove_context(std::thread::id _id)
{
    this->contexts.erase(_id);
}

void FightContext::on_frame()
{
    for (auto &it : this->actors) {
        it.second->update();
    }
}

void FightContext::register_actor(Actor *actor)
{
    int id = generate_id();
    actor->id = id;
    this->actors[id] = actor;
}

void FightContext::remove_actor(Actor *actor)
{
    auto it = this->actors.find(actor->id);
    if (it == this->actors.end()) {
        return;
    }

    this->actors.erase(actor->id);
    delete actor;
}


int FightContext::generate_id()
{
    int id = ++this->id;
    if (id >= MAX_ACTOR_AMOUNT) {
        // TODO
    }

    return id;
}
