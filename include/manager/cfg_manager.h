#ifndef __CFG_MANAGER_H__
#define __CFG_MANAGER_H__
#include <unordered_map>
#include "cfg/buff_cfg.h"
#include "cfg/equip_cfg.h"
#include "cfg/game_cfg.h"
#include "cfg/lottery_cfg.h"
#include "cfg/player_cfg.h"
#include "cfg/skill_cfg.h"
#include "cfg/hero_cfg.h"

class CfgManager
{
public:
    GameCfg game_cfg;
    BuffCfg buff_cfg;
    HeroCfg hero_cfg;
    SkillCfg skill_cfg;
    EquipCfg equip_cfg;
    LotteryCfg lottery_cfg;
    PlayerCfg player_cfg;

public:
    static CfgManager & get_instance();
    bool load_cfg();

    void set_config_path(const std::string &);
    const std::string & get_config_path() { return config_path; }
private:
    CfgManager();
    std::string config_path;
    std::unordered_map<std::string, CfgBase *> cfgs;
};

#endif