#include "game/fetter.h"
#include "game/game.h"
#include "game/player.h"
#include "manager/cfg_manager.h"
#include "game/lottery.h"
#include "game/hero.h"
#include "game/fight.h"
#include "cfg./hero_cfg.h"

void GamePlayer::update(float delta)
{
    for (auto &it : fetters) {
        it->update(delta);
    }
}

bool GamePlayer::on_fight_begin()
{
    for (auto &it : fetters) {
        if (it->init()) {
			return false;
		}
    }

    if (!cache_buffs.empty()) {
        for (auto &it : cache_buffs) {
            for (auto &it1 : this->boardHeros) {
                it1.second->add_buff(it);
            }
        }

        this->cache_buffs.clear();
        this->cache_buffs.shrink_to_fit();
    }

	return true;
}

void GamePlayer::on_fight_end(bool win, int rest)
{
	// TODO 计算伤害 & buff计算等
	if (this->own_game) {
		return;
	}

	this->own_game->on_fight_end();
}

void GamePlayer::add_unit_buff(BuffBase *buff)
{
    if (!buff) {
        return;
    }

    cache_buffs.push_back(buff);
}


void GamePlayer::on_changed()
{
	std::unordered_map<int, int> counter;
	for (auto &it : this->boardHeros) {
		if (!it.second->hero_cfg) {
			continue;
		}

		for (int id : it.second->hero_cfg->fetters) {
			++counter[id];
		}
	}


    bool first = true;
	for (auto it  = counter.begin(); it != counter.end(); ++it) {
        auto cfg = CfgManager::get_instance().hero_cfg.get_fetter(it->first);
        if (!cfg) {
            continue;   
        }

        int active = cfg->is_active(it->second);
        if (active < 0) {
            continue;
        }

        FetterBase *fetter = dynamic_cast<FetterBase *>(this->own_game->create_object(cfg->impl_name));
        if (!fetter) {
            continue;
        }

        if (first) {
            for (auto &it : this->fetters) {
				it->deinit();
                this->own_game->free_object(it);
            }

            first = false;
        }

        fetter->active = active;
		fetter->init();
        this->fetters.push_back(fetter);
    }
}

// 使用金币
bool GamePlayer::use_gold(int amount)
{
	if (this->gold - amount < 0 || (this->own_game->state != GameState::select_scene || this->own_game->state != GameState::select_weapon_with_hero)) {
		return false;
	}

	// TODO log
	this->gold -= amount;
	return true;
}

void GamePlayer::lottery_hero()
{
	if (!(this->own_game->state == GameState::idle_normal || this->own_game->state == GameState::idle_spec)) {
		return;
	}

	const auto &cfg = CfgManager::get_instance().player_cfg.misc;
	if (this->gold - cfg.refresh_cost < 0) {
		return;
	}

	this->lottery_candidate_heros.clear();
	std::vector<int> candidates;
	this->own_game->lottery_helper->lottery_heros(this->level, cfg.refresh_amount, candidates);
	if (candidates.size() != cfg.refresh_amount) {
		return;
	}

	for (auto id : candidates) {
		lottery_candidate_heros.push_back({id, false});
	}

	use_gold(cfg.refresh_cost);
}

void GamePlayer::select_hero(int idx)
{
	if (!(this->own_game->state == GameState::idle_normal || this->own_game->state == GameState::idle_spec) || this->lottery_candidate_heros.empty()) {
		return;
	}

	if (this->lottery_candidate_heros.size() <= idx || idx < 0 || this->lottery_candidate_heros[idx].second) {
		return;
	}

	const auto &cfg = CfgManager::get_instance().player_cfg.misc;
	
	int hero_id = this->lottery_candidate_heros[idx].first;
	FightUnit *unit = this->own_game->create_hero(hero_id);
	if (!unit) {
		return;
	}

	/*
		TODO
		先检查备战区的，如果有相同的星级英雄 > 2，则升级，否则再检查上场的， > 2 升级到场上，移除备战区的
		升级后再检查上场的
	*/
	this->lottery_candidate_heros[idx].second = true;
	std::vector<FightUnit *> same;
	int cnt1 = 0, cnt2 = 0;
	for (auto &it : this->holderHeros) {
		if (it.second->hero_cfg->hero_id == hero_id && it.second->star < 3) {
			same.push_back(it.second);
			if (it.second->star == 1) {
				++cnt1;
			} else {
				++cnt2;
			}
		}
	}

	int bcnt1 = 0, bcnt2 = 2;
	for (auto &it : this->boardHeros) {
		if (it.second->hero_cfg->hero_id == hero_id && it.second->star < 3) {
			if (it.second->star == 1) {
				++bcnt1;
			} else {
				++bcnt2;
			}
		}
	}

	if (cnt1 == 2) {
		++unit->star;

		bool rm_2 = false;
		if (cnt2 == 2) {
			rm_2 = true;
			unit->star = 3;
		} else {
			if (bcnt2 == 2) {
				unit->star = 3;
				// TODO

			}
		}

		for (auto &it : same) {
			if (!rm_2 && it->star == 2) {
				continue;
			} 

			this->holderHeros.erase(it->id);
		}
	} else {
		if (bcnt1 == 2) {
			++unit->star;
			if (bcnt2 == 2) {
				unit->star = 3;
				// TODO

			}
		} else {
			this->holderHeros[unit->id] = unit;
		}

		for (auto &it : same) {
			if (it->star == 2) {
				continue;
			} 

			this->holderHeros.erase(it->id);
		}
	}
}

// 上场英雄
void GamePlayer::push_hero(int id, Vector2 pos)
{
	if (!this->fight_obj || !(this->own_game->state == GameState::idle_normal || this->own_game->state == GameState::idle_spec)) {
		return;
	}

	auto it = this->boardHeros.find(id);
	auto hero_cfg = CfgManager::get_instance().hero_cfg.get_hero(id);

	if (it == this->boardHeros.end() || !hero_cfg) {
		return;
	}

	if (this->fight_obj->map->has_unit(pos)) {
		return;
	}

	this->fight_obj->map->set_hero(it->second, pos);
	it->second->pos = pos;
	this->holderHeros[it->second->id] = it->second;
	this->boardHeros.erase(it);
}

// 卖掉英雄
void GamePlayer::sell_hero(bool board, int id)
{
	if (!(this->own_game->state == GameState::idle_normal || this->own_game->state == GameState::idle_spec)) {
		return;
	}

	auto hero_cfg = CfgManager::get_instance().hero_cfg.get_hero(id);
	if (!hero_cfg) {
		return;
	}

	auto cost_cfg = CfgManager::get_instance().lottery_cfg.get_star_cost(hero_cfg->gold);
	if (!cost_cfg) {
		return;
	}

	auto *container = board ? &this->boardHeros : &this->holderHeros;

	auto it = container->find(id);
	if(it == this->boardHeros.end()) {
		return;
	}

	int star = it->second->star;
	if (star < 0 || star > 3) {
		return;
	}

	int gain = cost_cfg->sell[star - 1];

	// TODO log
	this->gold += gain;
	this->own_game->free_object(it->second);

	if (!board) {
		this->fight_obj->map->remove(it->second);
	}

	container->erase(it);
}

// 替换英雄
void GamePlayer::replace_hero(int id1, int id2)
{
	if (!(this->own_game->state == GameState::idle_normal || this->own_game->state == GameState::idle_spec)) {
		return;
	}

	auto it1 = this->boardHeros.find(id1);
	auto it2 = this->holderHeros.find(id2);
	if (it1 == this->boardHeros.end() || it2 == this->holderHeros.end()) {
		return;
	}

	this->fight_obj->map->remove(it2->second);
	this->fight_obj->map->set_hero(it1->second, it2->second->pos);
	it1->second->pos = it2->second->pos;
	it2->second->pos = {-1, -1};
	this->boardHeros[it2->second->id] = it2->second;
	this->holderHeros[it1->second->id] = it1->second;
	this->boardHeros.erase(it1);
	this->holderHeros.erase(it2);
}

// 放置装备
void GamePlayer::puton_equip(bool board, int hero_id, int equip_id)
{
	if (!(this->own_game->state == GameState::idle_normal || this->own_game->state == GameState::idle_spec)) {
		return;
	}

	auto *container = board ? &this->boardHeros : &this->holderHeros;
	auto it = container->find(hero_id);
	if (it == container->end()) {
		return;
	}

	// TODO

}



// 选择海克斯buff
void GamePlayer::select_buff(int idx)
{
	if (idx < 0 || idx >= this->candidate_buffs.size()) {
		return;
	}

	// TODO 设置战斗buff & 非战斗buff

}

// 获取战斗相关的buff
std::vector<BuffBase *> GamePlayer::get_fight_buffs()
{
	return fight_buffs;
}
