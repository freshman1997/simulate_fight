#ifndef __SKILL_CFG_H__
#define __SKILL_CFG_H__
#include <vector>
#include "cfg.h"
#include "game/skill.h"

struct SkillBuff
{
    int buff_id;
    std::vector<int> params;
};

struct Skill
{
    int skill_id;
    skill_type type;
    skill_perform_type perform_type;
    std::vector<SkillBuff> buffs;
    std::string name;
    std::string impl_name;
};

class SkillCfg : public CfgBase
{
public:
    virtual bool load();
};

#endif