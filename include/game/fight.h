#ifndef __FIGHT_H__
#define __FIGHT_H__
#include <unordered_map>
#include <vector>

#include "hero.h"
#include "actor.h"

class Game;

// 单个回合
class Fight : public Actor
{

public:
    Fight();
    virtual void update(float deltaTime);

public:
    bool IsBlocked();   // 用于外部检查是否失败，逻辑错误等等异常导致本场游戏无效

public:
    void clear_cache();
    // 进入战斗前激活已存在的buff，如海克斯、羁绊、装备的加成等
    void pre_enable_buffs();
    void fight();
    void round(FightUnit *);

    void random_units();
    
public:
    void push_fight_unit(FightUnit *, bool);
    bool is_end() const;
public:
    std::vector<FightUnit *> player1;
    std::vector<FightUnit *> player2;

    std::vector<FightUnit *> fight_actors;
    Game *game;
    bool started;
    bool error;
};

#endif