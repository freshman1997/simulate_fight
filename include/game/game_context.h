#ifndef __GAME_CONTEXT_H__
#define __GAME_CONTEXT_H__
#include <vector>
#include <unordered_map>
#include <thread>

class Actor;

const static int MAX_ACTOR_AMOUNT = 10000000;

class FightContext
{
public:
    void on_frame();
    void register_actor(Actor *);
    void remove_actor(Actor *);
    
public:
    unsigned long long lastUpdateTime;
    int deltaTime;

private:
    int generate_id();

private:
    int id;
    std::unordered_map<int, Actor *> actors;
};

class GameContext
{
public:
    static GameContext & get_game_context();
    FightContext * find_context(std::thread::id);
    void remove_context(std::thread::id);

private:
    std::unordered_map<std::thread::id, FightContext> contexts;
};

#endif