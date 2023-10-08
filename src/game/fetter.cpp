#include "game/fetter.h"
#include "cfg/hero_cfg.h"
#include "game/buff.h"
#include "manager/cfg_manager.h"
#include "game/player.h"
#include "game/game.h"

bool FetterBase::init()
{
    if (!player || !this->fetter_cfg || this->fetter_cfg->buff_amount.empty()) {
        return false;
    }

    auto it = this->fetter_cfg->buff_amount.find(this->active);
    if (it == this->fetter_cfg->buff_amount.end()) {
        return false;
    }

    for (auto &buff_item : it->second) {
        auto buff_cfg = CfgManager::get_instance().buff_cfg.get_buff(buff_item.first);
        if (!buff_cfg) {
            continue;
        }

        BuffBase *buff = dynamic_cast<BuffBase *>(this->player->own_game->create_object(buff_cfg->impl_name));
        if (!buff) {
            continue;
        }

        if (!buff_item.second.empty()) {
            buff->lasting = buff_item.second[0];
            std::copy(buff_item.second.begin() + 1, buff_item.second.end(), buff->params.begin());
        }
        this->player->add_unit_buff(buff);
    }
    
    return true;
}