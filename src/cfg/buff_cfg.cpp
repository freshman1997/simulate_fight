#include "cfg/buff_cfg.h"
#include "cfg/cfg.h"

bool BuffCfg::load()
{
    //READ_CFG("buff.json", &BuffCfg::load_config)
    return true;
}

bool BuffCfg::load_config(json &json_data)
{
    if (!json_data.is_object()) {
        return false;
    }

    for (auto &[key, val] : json_data.items()) {
        if (key.empty()) {
            return false;
        }

        if (!val.is_object()) {
            return false;
        }

        int t = val["sub_type"];
        std::cout << key << ": " << val["sub_type"] << std::endl;
    }

    return true;
}
