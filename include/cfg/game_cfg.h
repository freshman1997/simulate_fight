#ifndef __GAME_CFG_H__
#define __GAME_CFG_H__
#include "cfg.h"
#include "nlohmann/json_fwd.hpp"
#include <unordered_map>
#include <utility>
#include <vector>

struct game_misc
{
    int id;
    int frame_rate;
    int quick_frame_rate;
    int surrender_phase;
};

struct game_phase
{
    int pid;
    int type;
    int amount;
    int exp;
    int time;
    std::vector<int> monsters;
    std::string desc;
};

struct pve_monster
{
    int id;
    int monster_id;
    int star;
    int skill_id;
    int position[2];
    std::vector<std::pair<int, std::vector<float>>> buffs;
};

struct level_cfg
{
    int lv;
    int exp;
    int amount;
    std::vector<float> poll_ratio;
};

class GameCfg : public CfgBase
{
public:
    virtual bool load() override;

    int get_frame_rate() const;
    int get_quick_frame_rate() const;
    const game_phase * get_phase(int id);
    const pve_monster * get_pve_monster(int id);

private:
    bool parse_misc(json &);
    bool parse_phase(json &);
    bool parse_pve_monster(json &);
    bool parse_level_config(json &);

private:
    game_misc misc;
    std::unordered_map<int, game_phase> phases;
    std::unordered_map<int, pve_monster> pve_monsters;
};

#endif
