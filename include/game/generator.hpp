#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__
#include <unordered_map>
#include <string>

#include "skill.h"
#include "equip.h"
#include "hero.h"

#define XX(p) ( ## p)

class Generator
{
private:
    Generator() {}
    std::unordered_map<std::string, SkillBase *> skill_types;
    std::unordered_map<std::string, EquipBase *> equip_types;
    std::unordered_map<std::string, HeroBase *> hero_types;

public:
    static Generator & get_instance();
    SkillBase * new_skill_instrance(const std::string &);
    EquipBase * new_equip_instrance(const std::string &);
    HeroBase * new_hero_instrance(const std::string &);


    template<class T, class BaseType>
    bool register_type(const std::string &key)
    {
        static_assert(std::is_base_of<BaseType, T>::value, "T must base from SkillBase or EquipBase or HeroBase");

        if (std::is_base_of<SkillBase, T>::value) {
            if (skill_types.count(key)) {
                return false;
            }

            skill_types[key] = new T;
        } else if (std::is_base_of<EquipBase, T>::value) {
            if (equip_types.count(key)) {
                return false;
            }

            equip_types[key] = new T;
        } else if (std::is_base_of<HeroBase, T>::value) {
            if (hero_types.count(key)) {
                return false;
            }

            hero_types[key] = new T;
        } else {
            return false;
        }
        
        return true;
    }
};

#define REGISTER_TYPE(_name, type, base) namesapce { \
    auto val = SkillGenerator::get_instance().register_type<type, base>(_name); \
}

#endif
