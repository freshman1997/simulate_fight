#include "cfg/buff_cfg.h"
#include "cfg/cfg.h"

bool BuffCfg::load()
{
    //READ_CFG("buff.json", &BuffCfg::load_config)
    return true;
}

bool BuffCfg::parse_buff_cfg(json &json_data)
{
    if (!json_data.is_object()) {
        return false;
    }

    for (auto &it : json_data) {
        Buff buf;
        buf.id = -1;
        if (it["id"].is_number_integer()) {
            buf.id = it["id"];
        }
        
        buf.functype = -1;
        if (it["func_type"].is_number_integer()) {
            buf.functype = (int)it["func_type"];
        }
        
        buf.trigger_cond = -1;
        if (it["trigger_condition"].is_number_integer()) {
            buf.trigger_cond = (int)it["trigger_condition"];
        }

        buf.trigger_time = -1;
        if (it["trigger_condition"].is_number_integer()) {
            buf.trigger_time = it["trigger_condition"];
        }

        buf.trigger_rate = 0;
        if (it["trigger_rate"].is_array()) {
            if (it["trigger_rate"].size() == 2) {
                buf.trigger_rate = 1.0f * it["trigger_rate"][0].get<int>() / it["trigger_rate"][0].get<int>();
            } else {
                // TODO
            }
        }

        buf.die_keep = false;
        if (it["keep"].is_number_integer()) {
            buf.die_keep = it["keep"];
        }

        if (it["impl_name"].is_string()) {
            buf.impl_name = it["impl_name"];
        }

        this->buffs[buf.id] = buf;
    }

    return true;
}

const Buff * BuffCfg::get_buff(int id)
{
    auto it = this->buffs.find(id);
    return it == this->buffs.end() ? nullptr : &it->second;
}
