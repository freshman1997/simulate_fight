#ifndef __SKILL_CFG_H__
#define __SKILL_CFG_H__
#include <vector>
#include "cfg.h"

struct SkillBuff
{
    int buff_id;
    std::vector<int> params;
};

struct Skill
{
    int skill_id;
    int type;
    int perform_type;
    int distance;
    int skill_time;
    int skill_sing_time;
    std::vector<SkillBuff> buffs;
    std::string name;
    std::string impl_name;
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