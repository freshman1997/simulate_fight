#ifndef __GAME_CFG_H__
#define __GAME_CFG_H__
#include "cfg.h"
#include <unordered_map>

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
    std::unordered_map<int, std::pair<int, int>> monster_pos;
    std::string desc;
};

class GameCfg : public CfgBase
{
public:
    virtual bool load() override;

    int get_frame_rate() const;
    int get_quick_frame_rate() const;
    const game_phase * get_phase(int id) const;
private:
    bool parse_misc(json &);
    bool parse_phase(json &);

private:
    game_misc misc;
    std::unordered_map<int, game_phase> phases;
};

#endif
