#include "game/fetters/demasia.h"
#include "cfg/hero_cfg.h"

#include "game/fetter.h"
#include "game/hero.h"
#include "game/buff.h"
#include "game/player.h"
#include "manager/object_manager.hpp"
#include <algorithm>

REGISTER_TYPE("demasia", Demasia)

Object * Demasia::clone()
{
    return new Demasia;
}

Object * Demasia::clone_and_clean()
{
    return clone();
}

void Demasia::deinit()
{

}

bool Demasia::init()
{
    if (!this->player || this->active < 0) {
        return false;
    }

    for (auto &it : this->player->boardHeros) {
        auto fetter_it =  std::find(it.second->hero_cfg->fetters.begin(), it.second->hero_cfg->fetters.end(), (int)fetter_type::demasia);
        if (fetter_it != it.second->hero_cfg->fetters.end()) {
            // TODO
            
        }
    }

    return true;
}

void Demasia::update(float deltaTime)
{

}

void Demasia::on_begin()
{

}

// 正式开启战斗之前
void Demasia::on_before_round()
{
    
}

void Demasia::generate_rand_equips()
{
    
}

