#include "game/generator.hpp"

Generator & Generator::get_instance() 
{
    static Generator instance;
    return instance;
}

SkillBase * Generator::new_skill_instrance(const std::string &key)
{
    auto it = this->skill_types.find(key);
    if (it == this->skill_types.end()) {
        return nullptr;
    }

    return it->second->clone();
}

EquipBase * Generator::new_equip_instrance(const std::string &key)
{
    auto it = this->equip_types.find(key);
    if (it == this->equip_types.end()) {
        return nullptr;
    }

    return it->second->clone();
}

HeroBase * Generator::new_hero_instrance(const std::string &key)
{
    auto it = this->hero_types.find(key);
    if (it == this->hero_types.end()) {
        return nullptr;
    }

    return it->second->clone();
}
