#ifndef __SKILL_CFG_H__
#define __SKILL_CFG_H__
#include <unordered_map>
#include <vector>
#include "cfg.h"
#include "nlohmann/json_fwd.hpp"

struct SkillBuff
{
    int buff_id;
    std::vector<float> params;
};

struct Skill
{
    int skill_id;
    int perform_type;
    int distance;
    int skill_time;
    int skill_sing_time;
    int target_choose_rule;
    int target_amount;
    int fly_speed;
    int fly_width;
    int passive_cond;
    std::unordered_map<int, std::vector<SkillBuff>> buffs;
    std::unordered_map<int, std::vector<SkillBuff>> passive_buffs;
    std::string name;
    std::string impl_name;
    json extra_params;
};

class SkillCfg : public CfgBase
{
public:
    virtual bool load();
    bool parse_skill(json &doc);
    const Skill * get_skill(int id);

private:
    std::unordered_map<int, Skill> skills;
};

#endif