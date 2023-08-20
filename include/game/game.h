#ifndef __GAME_H__
#define __GAME_H__
#include <vector>
#include <memory>

#include "actor.h"

enum class GameState
{
    none,
    select_scene,                           // 选择当前场的场景
    select_weapon_with_hero,                // 选秀
    idle_normal,                            // 普通pve、pvp结束后中场
    idle_spec,                              // 选择海克斯的中场
    fight,                                  // 战斗期，pve & pvp             
};

class HeroBase;
class FightContext;
class Fight;
class EquipBase;
class InputCommandBuffQueue;
class OutputFrameBuffQueue;

struct EquipHolder
{
    Vector2 pos;
    EquipBase *equip;
};

// 单个玩家
class GamePlayer
{
public:
    GamePlayer();
    bool use_gold(int amount);
    void defeat(int rest_heros);

    // 上场英雄
    void push_hero(bool board, HeroBase *);
    // 卖掉英雄
    void sell_hero(bool board, Vector2 pos);
    // 替换英雄
    void replace_hero(Vector2 bpos, Vector2 hpos);

    // 放置装备
    void puton_equip(bool board, Vector2 epos, Vector2 hpos);
public:
    // 开始战斗前的初始化工作
    void pre_start();

public:
    int player_id;                          // 玩家id
    int hp;                                 // 当前血量
    int gold;                               // 金币数量
    int win_amount;                         // 当前胜利场数 
    int defeat_amount;                      // 当前被打败场数
    int speed;                              // 小小英雄移速
    int interest;                           // 当前利息，海克斯可以修改
    Vector2 pos;                            // 当前所在位置
    std::vector<EquipHolder> equip_holder;  // 装备
    std::vector<HeroBase *> boardHeros;     // 棋盘上的棋子
    std::vector<HeroBase *> holderHeros;    // 备战区的棋子
};

#define FRAME(frame_rate) (1.0f / frame_rate * 1000)
class Timer;

// 单场比赛对象
class Game : public std::enable_shared_from_this<Game>
{

public:
    Game();
    bool start_game();   // 开始战斗前的准备工作
    void end_game();

    void game_loop();

    void continue_game();
    void pause_game();

    // TODO
    ~Game() {}
private:
    bool update(float delta);

    void start_pvp();
    void start_pve();

    void clear_cache();

public:
    int get_frame_time();
    bool is_all_fight_end();

public:
    int id;                                     // 当前场次的id
    int round;                                  // 当前是第几场，如3-2
    int round_end_count;                        // 当前回合结束的场数
    bool pause;                                 // 暂停标志
    bool resume;                                // 恢复标志
    long long start_time;                       // 开始游戏时间点
    long long end_time;                         // 结束游戏时间点
    long long last_time;                        // 上次执行的时间
    int state_pass_time;                        // 当前状态已经过去的时间
    int frame_rate;                             // 帧率
    int quick_frame_rate;                       // 2倍速
    int cur_frame_time;                         // 当前帧率
    bool normal_rate;                           // 是否处于加速状态
    int next_time;                              // 下次执行间隔时间
    GameState state;                            // 游戏当前阶段
    GamePlayer players[8];                      // 当前场次的玩家
    FightContext *ctx;                          // 当前场次的上下文
    long long state_start_time;                 // 开始战斗时间点
    bool on_fight;                              // 是否处于战斗状态
    std::vector<Fight *> round_objects;         // 战斗回合对象, 最多8个，最少1个
    InputCommandBuffQueue *input;               // 输入队列
    OutputFrameBuffQueue *output;               // 输出队列
    Timer *timer = nullptr;                     // 定时器对象
};

#endif