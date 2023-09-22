#ifndef __GAME_H__
#define __GAME_H__
#include <crtdefs.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>

#include "actor.h"
#include "cfg/game_cfg.h"
#include "game/actor.h"
#include "io/input_command_buff_queue.h"
#include "io/output_frame_buff_queue.h"
#include "player.h"

#define FRAME(frame_rate) (1.0f / frame_rate * 1000)

enum class GameState
{
    none = -1,
    select_scene,                           // 选择当前场的场景
    select_weapon_with_hero,                // 选秀
    idle_normal,                            // 普通pve、pvp结束后中场
    idle_spec,                              // 选择海克斯的中场
    fight_pve,                              // 战斗期，pve
    fight_pvp,                              // 战斗期，pvp             
    force_end_game,                         // 强制结束
};

class HeroBase;
class Fight;
class EquipmentBase;
class EventManager;
struct game_phase;
class FightUnit;
class Game;
class BuffBase;
class LotteryHelper;
class FetterBase;

class Timer;

// 单场比赛对象
class Game
{

public:
    Game();

    void set_players(std::vector<int> players);

    bool start_game();   // 开始战斗前的准备工作
    void end_game();

    void game_loop();

    void continue_game();
    void pause_game();

    // TODO
    ~Game() {}

    void test_game(GameState state, int);

private:
    bool update(float delta);

    void start_pvp();
    void start_pve();

    void clear_cache();
    bool is_ending();
    bool change_state(GameState state);
    bool is_the_turn(GameState state);

    int gid;
public:
    int get_frame_time();
    void on_fight_end();
    bool is_all_fight_end();

public:
    int get_gid();
    Object * create_object(const std::string &);
    void free_object(Object *obj);

public:    
    FightUnit * create_hero(int hero_id);

public:
    int scene_id;                               // 开始选中的阵营
    bool has_interest;                          // 是否有利息

public:
    int id;                                             // 当前场次的id
    const game_phase *phase;                            // 当前阶段对应的配置信息
    int round;                                          // 当前是第几场，如3-2
    int round_end_count;                                // 当前回合结束的场数
    bool pause;                                         // 暂停标志
    time_t pause_time;                                  // 暂停时间点
    time_t start_time;                                  // 开始游戏时间点
    time_t end_time;                                    // 结束游戏时间点
    time_t frame_total_time;                            // 上次执行的时间
    int state_pass_time;                                // 当前状态已经过去的时间
    int frame_rate;                                     // 帧率
    int quick_frame_rate;                               // 2倍速
    int cur_frame_time;                                 // 当前帧率
    bool normal_rate;                                   // 是否处于加速状态
    int next_time;                                      // 下次执行间隔时间
    GameState state;                                    // 游戏当前阶段
    GamePlayer players[8];                              // 当前场次的玩家
    time_t state_start_time;                            // 开始战斗时间点
    bool on_fight;                                      // 是否处于战斗状态
    std::vector<Fight *> round_objects;                 // 战斗回合对象, 最多8个，最少1个
    Timer *timer = nullptr;                             // 定时器对象
    EventManager *ev_manager = nullptr;                 // 事件管理器
    std::shared_ptr<LotteryHelper> lottery_helper;      // 抽奖
    InputCommandBuffQueue input;                        // 输入队列
    OutputFrameBuffQueue output;                        // 输出队列
};

#endif