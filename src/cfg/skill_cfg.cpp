#include "cfg/skill_cfg.h"

bool SkillCfg::load()
{
    READ_CFG("skill.json", &SkillCfg::parse_skill)
    return true;
}

bool SkillCfg::parse_skill(json &doc)
{
    Skill sk;
    sk.buffs.clear();

    for (auto &it : doc) {
        sk.skill_id = -1;
        if (it["skill_id"].is_number_integer()) {
            sk.skill_id = it["skill_id"];
        }

        if (it["name"].is_string()) {
            sk.name = it["name"];
        }

        sk.perform_type = -1;
        if (it["perform_type"].is_number_integer()) {
            sk.perform_type = it["perform_type"];
        }

        sk.type = -1;
        if (it["type"].is_number_integer()) {
            sk.type = it["type"];
        }

        sk.distance = 0;
        if (it["distance"].is_number_integer()) {
            sk.distance = it["distance"];
        }

        if (it["impl_name"].is_string()) {
            sk.impl_name = it["impl_name"];
        }

        if (it["buffs"].is_array()) {
            // TODO
            json &arr = it["buffs"];
            for (int i = 0; i < arr.size(); i++) {
                json &buf = arr[i];
                if (buf.is_array()) {
                    SkillBuff sb;
                    if (buf.size() > 1) {
                        sb.buff_id = buf[0];
                        for (int j = 1; j < buf.size(); ++j) {
                            if (buf[i].is_number_integer()) {
                                sb.params.push_back(buf[i]);
                            }
                        }

                        sk.buffs.push_back(sb);
                    }
                }
            }
        }

        this->skills[sk.skill_id] = sk;
    }

    return true;
}

const Skill * SkillCfg::get_skill(int id)
{
    auto it = this->skills.find(id);
    return it == this->skills.end() ? nullptr : &it->second;
}
