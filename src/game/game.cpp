#include <algorithm>
#include <crtdefs.h>
#include <iterator>
#include <memory>
#include <stdlib.h>
#include <thread>
#include <unordered_map>
#include <vector>
#include <random>

#include "game/actor.h"
#include "game/buff.h"
#include "game/event/event.h"
#include "game/game.h"
#include "game/fight.h"
#include "game/hero.h"
#include "game/skill.h"
#include "manager/object_manager.hpp"
#include "timer/timer.h"
#include "manager/cfg_manager.h"
#include "time/time.h"
#include "cfg/game_cfg.h"
#include "game/fight.h"
#include "game/lottery.h"

//----------------------- game -------------------------//
Game::Game() : round(1), pause(false), start_time(0), state(GameState::none), state_start_time(0), on_fight(false), normal_rate(false), next_time(0)
{
	const CfgManager &mgr = CfgManager::get_instance();
	frame_rate = mgr.game_cfg.get_frame_rate();
	quick_frame_rate = mgr.game_cfg.get_quick_frame_rate();
	cur_frame_time = (int)FRAME(frame_rate);
	round_end_count = 0;
	state_pass_time = 0;
	gid = 0;
	end_time = 0;
	ev_manager = new EventManager;
	frame_total_time = 0;
	lottery_helper = std::make_shared<LotteryHelper>();
}

void Game::set_players(std::vector<int> players)
{
	if (players.empty() || players.size() > 8) {
		return;
	}

	for (int i = 0; i < players.size(); ++i) {
		this->players[i].player_id = id;
		this->players[i].own_game = this;
	}
}

int Game::get_gid()
{
	return gid++;
}

Object * Game::create_object(const std::string &impl_name)
{
	Object *res = ObjectManager::get_instance().clone_one_clean_object(impl_name);
	if (!res) {
		return nullptr;
	}

	res->id = get_gid();

	return res;
}

void Game::free_object(Object *obj)
{
	if (!obj) {
		return;
	}

	Actor *actor = dynamic_cast<Actor *>(obj);
	if (actor) {
		actor->deinit();
	}

	ObjectManager::get_instance().release_object(obj);
}

FightUnit * Game::create_hero(int hero_id)
{
	auto hero_cfg = CfgManager::get_instance().hero_cfg.get_hero(hero_id);
	if (!hero_cfg) {
		return nullptr;
	}

	FightUnit *unit = (FightUnit *)create_object(hero_cfg->impl_name);
	if (!unit) {
		return nullptr;
	}

	unit->init();

	return unit;
}

bool Game::is_the_turn(GameState state)
{
	const game_phase *tmp_phase = CfgManager::get_instance().game_cfg.get_phase(round);
	if (!tmp_phase) {
		return false;
	}

	return tmp_phase->type == (int)state;
}

bool Game::change_state(GameState state)
{
	this->state = state;
	this->state_start_time = datetime::now();
	++this->round;
	this->phase = CfgManager::get_instance().game_cfg.get_phase(round);
	if (!this->phase) {
		return false;
	}

	this->next_time = this->phase->time * 1000;
	return true;
}

void Game::on_fight_end()
{
	++round_end_count;
}

bool Game::is_all_fight_end()
{
	return round_end_count == round_objects.size();
}

void Game::pause_game()
{
	pause = true;
	pause_time = datetime::now();
}

void Game::continue_game()
{
	pause = false;
	timer->AddTimer(this->next_time - pause_time, [this](const TimerNode &node){
    	game_loop();
    });
	
	pause_time = 0;
}

void Game::game_loop()
{
	if (pause || end_time != 0) {
		return;
	}

	time_t frame_time = -1;
	GameState last_state = state;

	switch (state)
	{
	case GameState::select_scene: {
		state = GameState::select_weapon_with_hero;
		break;
	}
	case GameState::select_weapon_with_hero:
	case GameState::idle_normal:
	case GameState::idle_spec: {
		// 切换状态，准备战斗数据
		if (is_the_turn(GameState::fight_pvp)) {
			state = GameState::fight_pvp;
			start_pvp();
		} else {
			start_pve();
			state = GameState::fight_pve;
		}

		if (!on_fight) {
			end_game();
			return;
		}
		break;
	}
	case GameState::fight_pve:
	case GameState::fight_pvp: {
		if (is_ending()) {
			// TODO
			end_game();
			return;
		}

		frame_time = (time_t)get_frame_time();
		this->frame_total_time += frame_time;
		if (this->frame_total_time >= this->next_time) {
			state = GameState::idle_normal;
			if (is_the_turn(GameState::idle_spec)) {
				state = GameState::idle_spec;
			}

			frame_time = -1;
			this->frame_total_time = 0;
		}
		break;
	}

	default:
		// TODO
		return;
	}

	if (last_state != state) {
		// state change, 结算利息
		if (!change_state(state)) {
			// TODO
			return;
		}
	}

	if (state == GameState::fight_pvp || state == GameState::fight_pve) {
		if (frame_time > 0) {
			bool result = this->update((float)frame_time);
			if (!result && !pause) {
				end_game();
				return;
			}
		} else {
			frame_time = (time_t)get_frame_time();
		}
	}

	std::cout << "game frame: " << frame_time << ", " << this->frame_total_time << ", " << this->next_time << std::endl;

	timer->AddTimer(frame_time >= 0 ? frame_time : this->next_time, [this](const TimerNode &node){
    	game_loop();
    });
}

void Game::clear_cache()
{

}

bool Game::is_ending()
{
	return round_end_count == 8;
}


bool Game::start_game()
{
	start_time = datetime::now();
	if (!change_state(GameState::select_scene)) {
		return false;
	}

	// 产生随机阵容
	// 计算时间到第一回合
	timer->AddTimer(next_time, [this](const TimerNode &node){
    	game_loop();
    });
	return true;
}

void Game::end_game()
{
	// TODO 
	end_time = 0;
}

bool Game::update(float delta)
{
	if (pause) {
		return false;
	}

	try {
		for (auto &it : round_objects) {
			if (it->is_end()) {
				continue;
			}

			it->update(delta);
		}

		for (int i = 0; i < 8; ++i) {
			players[i].update(delta);
		}

		return true;
	} catch(...) {
		return false;
	}
}

void Game::start_pvp()
{
	std::vector<int> alive_players;
	for (int i = 0; i < 8; ++i) {
		if (players[i].hp > 0) {
			alive_players.push_back(i);
		}
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(alive_players.begin(), alive_players.end(), g);
	
	for (int i = 0; i < alive_players.size(); i += 2) {
		GamePlayer *player1 = &players[alive_players[i]];
		GamePlayer *player2 = nullptr;
		bool mirror = false;

		if (i + 1 < alive_players.size()) {
			player2 = &players[alive_players[i + 1]];
		} else {
			player2 = &players[alive_players[rand() % alive_players.size()]];
			mirror = true;
		}

		Fight *fight = new Fight(this);
		player1->fight_obj = fight;
		player2->fight_obj = fight;

		for (auto &it : player1->boardHeros) {
			fight->push_fight_unit(it.second, true);
		}

		for (auto &it : player2->boardHeros) {
			fight->push_fight_unit(it.second, false);
		}

		fight->is_mirror = mirror;
		fight->p1 = player1;
		fight->p2 = player2;
		if (!fight->init()) {
			on_fight = false;
			return;			
		}

		this->round_objects.push_back(fight);
	}

	on_fight = true;
}

void Game::start_pve()
{
	for (int i = 0; i < 8; ++i) {
		GamePlayer &player = players[i];
		if (player.hp <= 0) continue;

		Fight *fight = new Fight(this);
		player.fight_obj = fight;

		for (int id : this->phase->monsters) {
			const pve_monster *monster_cfg  = CfgManager::get_instance().game_cfg.get_pve_monster(id);
			if (!monster_cfg) {
				continue;
			}

			const Hero *hero_cfg = CfgManager::get_instance().hero_cfg.get_hero(monster_cfg->monster_id);
			if (!hero_cfg) {
				continue;
			}

			HeroBase *unit = dynamic_cast<HeroBase *>(create_object(hero_cfg->impl_name));
			if (!unit) {
				continue;
			}

			// TODO buff等数值
			for (auto &it : monster_cfg->buffs) {
				const Buff *buff_cfg =  CfgManager::get_instance().buff_cfg.get_buff(it.first);
				if (!buff_cfg) {
					continue;
				}

				BuffBase *buff = dynamic_cast<BuffBase *>(create_object(buff_cfg->impl_name));
				if (!buff) {
					continue;
				}

				if (!it.second.empty()) {
					buff->lasting = it.second[0];
					std::copy(it.second.begin() + 1, it.second.end(), buff->params.begin());
				}

				unit->add_buff(buff);
			}

			if (monster_cfg->skill_id) {
				const Skill * skill_cfg = CfgManager::get_instance().skill_cfg.get_skill(monster_cfg->skill_id);
				if (skill_cfg) {
					SkillBase *skill = dynamic_cast<SkillBase *>(create_object(skill_cfg->impl_name));
					if (skill) {
						unit->skill = skill;
					}
				}
			}

			unit->hero_cfg = hero_cfg;
			unit->star = monster_cfg->star;
			unit->pos = {(float)monster_cfg->position[0], (float)monster_cfg->position[1]};
			
			fight->p2 = &player;
			fight->push_fight_unit(unit, false);
		}

		for (auto &it : player.boardHeros) {
			fight->push_fight_unit(it.second, true);
		}

		if (!fight->init()) {
			on_fight = false;
			return;			
		}

		this->round_objects.push_back(fight);
	}
	
	on_fight = true;
}

int Game::get_frame_time()
{
	return (int)(!normal_rate ? FRAME(frame_rate) : FRAME(quick_frame_rate));
}

void Game::test_game(GameState state, int round)
{
	this->round = round;
	change_state(state);
	timer->AddTimer(next_time, [this](const TimerNode &node){
    	game_loop();
    });
}
