#include "cfg/game_cfg.h"

bool GameCfg::load()
{
    READ_CFG("fight_misc.json", &GameCfg::parse_misc)
    READ_CFG("fight_phase.json", &GameCfg::parse_phase)
    return true;
}

bool GameCfg::parse_misc(json &doc)
{
    auto it = doc.begin();
    if (!it->is_object()) {
        return false;
    }

    this->misc.id = it->at("id");
    this->misc.frame_rate = it->at("frame_rate");
    this->misc.quick_frame_rate = it->at("quick_frame_rate");
    this->misc.surrender_phase = it->at("surrender_phase");

    return true;
}

bool GameCfg::parse_phase(json &doc)
{
    if (doc.empty()) {
        return false;
    }

    game_phase tmp;
    for (auto &it : doc) {
        if (!it["pid"].is_number_integer()) {
            // error
            return false;
        }

        /*if (!it["monsters"].is_object()) {
            return false;
        }*/

        tmp.pid = it["pid"];
        if (it["amount"].is_number_integer()) {
            tmp.amount = it["amount"];
        }

        if (it["desc"].is_string()) {
            tmp.desc = it["desc"];
        }

        tmp.time = 0;
        if (it["time"].is_number_integer()) {
            tmp.time = it["time"];
        }

        tmp.type = -1;
        if (it["type"].is_number_integer()) {
            tmp.type = it["type"];
        }

        tmp.exp = 0;
        if (it["exp"].is_number_integer()) {
             tmp.exp = it["exp"];
        }
        
        /*for (auto &it1 : it["monsters"]) {

        }*/

        this->phases[tmp.pid] = tmp;
    }

    return true;
}

int GameCfg::get_frame_rate() const
{
    return misc.frame_rate;
}

int GameCfg::get_quick_frame_rate() const
{
    return misc.quick_frame_rate;
}

const game_phase * GameCfg::get_phase(int id) const
{
    auto it = this->phases.find(id);
    return it == this->phases.end() ? nullptr : &it->second;
}