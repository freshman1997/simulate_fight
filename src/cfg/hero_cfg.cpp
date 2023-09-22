#include "cfg/hero_cfg.h"
#include <algorithm>

int Fetter::is_active(int amount) const
{
    if (buff_amount.empty()) {
        return -1;
    }

    int res = -1;
    for (auto &it : buff_amount) {
        if (it.first <= amount) {
            res = std::max(it.first, res);
        }
    }

    return res;
}


bool HeroCfg::load()
{
    READ_CFG("hero.json", &HeroCfg::parse_hero_cfg)
    return true;
}

bool HeroCfg::parse_hero_cfg(json &)
{
    // TODO
    return true;
}

const Hero * HeroCfg::get_hero(int id)
{
    auto it = this->heros.find(id);
    return it == this->heros.end() ? nullptr : &it->second;
}

const Fetter * HeroCfg::get_fetter(int id)
{
    auto it = this->fetters.find(id);
    return it == this->fetters.end() ? nullptr : &it->second;
}


void HeroCfg::for_each_heros(std::function<bool(const Hero &)> fun)
{
    for (auto &it : heros) {
        if (!fun(it.second)) {
            break;
        }
    }
}

