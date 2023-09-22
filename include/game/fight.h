#ifndef __FIGHT_H__
#define __FIGHT_H__
#include <memory>
#include <unordered_map>
#include <vector>

#include "hero.h"
#include "map.h"
#include "actor.h"

class Game;
class GamePlayer;

// 单个回合
class Fight : public Actor
{

public:
    Fight(Game *game);
    virtual void update(float deltaTime);
    virtual Object * clone();
    virtual Object * clone_and_clean();

public:
    bool IsBlocked();   // 用于外部检查是否失败，逻辑错误等等异常导致本场游戏无效

public:
    void clear_cache();
    // 进入战斗前激活已存在的buff，如海克斯、羁绊、装备的加成等
    virtual bool init();
    virtual void deinit();
    void random_units();
    void on_fight_end();

public:
    void push_fight_unit(FightUnit *, bool);
    bool is_end() { return win_side >= 0; }

public:
    GamePlayer *p1;
    GamePlayer *p2;
    std::vector<FightUnit *> player1;
    std::vector<FightUnit *> player2;

    std::vector<FightUnit *> fight_actors;
    std::shared_ptr<GameMap>  map;
    Game *game;
    bool is_mirror;
    bool error;
    int die_counter[2];
    int win_side;
    int rest_heros;     // 结束后胜利方剩余单位数
};

#endif