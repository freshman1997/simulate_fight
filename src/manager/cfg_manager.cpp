#include "manager/cfg_manager.h"

#define __REGISTER_CFG__(cfg) do { cfg.set_cfg_name(#cfg); this->cfgs[#cfg] = &cfg; } while(false);

CfgManager::CfgManager()
{
    __REGISTER_CFG__(buff_cfg)
    __REGISTER_CFG__(game_cfg)
    __REGISTER_CFG__(skill_cfg)
    __REGISTER_CFG__(hero_cfg)
    __REGISTER_CFG__(equip_cfg)
    __REGISTER_CFG__(lottery_cfg)
}

CfgManager & CfgManager::get_instance()
{
    static CfgManager cfg_mgr;
    return cfg_mgr;
}

void CfgManager::set_config_path(const std::string &_path)
{
    this->config_path = _path;
    for (auto &it : cfgs) {
        it.second->set_config_path(this->config_path.c_str());
    }
}

bool CfgManager::load_cfg()
{
    for (auto &it : cfgs) {
        if (!it.second->load()) {
            return false;
        }
    }

    return true;
}
