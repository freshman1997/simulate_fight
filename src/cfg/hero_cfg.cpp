#include "cfg/hero_cfg.h"

bool HeroCfg::load()
{
    READ_CFG("heros.json", &HeroCfg::parse_hero_cfg)
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
