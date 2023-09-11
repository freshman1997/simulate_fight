#include <algorithm>
#include <crtdefs.h>
#include <iterator>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <random>

#include "game/actor.h"
#include "game/buff.h"
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

GamePlayer::GamePlayer() : player_id(0), hp(0), gold(0), win_amount(0), defeat_amount(0), interest(0)
{}

void GamePlayer::pre_start()
{
	
}

bool GamePlayer::use_gold(int amount)
{
	if (this->gold - amount < 0) {
		return false;
	}

	// TODO log
	this->gold -= amount;
	return true;
}

void GamePlayer::defeat(int rest_heros)
{

}

// 上场英雄
void GamePlayer::push_hero(bool board, HeroBase *)
{

}

// 卖掉英雄
void GamePlayer::sell_hero(bool board, Vector2 pos)
{

}

// 替换英雄
void GamePlayer::replace_hero(Vector2 bpos, Vector2 hpos)
{

}

// 放置装备
void GamePlayer::puton_equip(bool board, Vector2 epos, Vector2 hpos)
{

}

void GamePlayer::on_end_fight(bool win, int rest)
{

}

// 选择海克斯buff
void GamePlayer::select_buff(int idx)
{

}

// 获取战斗相关的buff
std::vector<int> GamePlayer::get_fight_buffs()
{
	return {};
}

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
}

int Game::get_gid()
{
	return gid++;
}

bool Game::is_the_turn(GameState turn)
{
	const game_phase *tmp_phase = CfgManager::get_instance().game_cfg.get_phase(round + 1);
	if (!tmp_phase) {
		return false;
	}

	return tmp_phase->type == (int)GameState::idle_spec;
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

bool Game::is_all_fight_end()
{
	if (round_end_count == round_objects.size()) {
		return true;
	}

	round_end_count = 0;
	for (auto &it : round_objects) {
		if (it->is_end()) {
			++round_end_count;
		}
	}

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
		break;
	}
	case GameState::fight_pve:
	case GameState::fight_pvp: {
		if (is_ending()) {
			// TODO
			end_game();
			return;
		}

		if (!is_all_fight_end()) {
			
		} else {
			state = GameState::idle_spec;
			if (is_the_turn(GameState::idle_spec)) {
				state = GameState::idle_spec;
			}
		}
		break;
	}

	default:
		// TODO
		return;
	}

	if (last_state != state) {
		// state change, 结算利息
		if (change_state(state)) {
			// TODO
			return;
		}
	}

	time_t frame_time = -1;
	if (state == GameState::fight_pvp || state == GameState::fight_pve) {
		frame_time = (time_t)get_frame_time();
		bool result = this->update((float)frame_time);
		if (!result && !pause) {
			end_game();
			return;
		}
	}

	timer->AddTimer(frame_time >= 0 ? frame_time : this->next_time, [this](const TimerNode &node){
    	game_loop();
    });
}

void Game::clear_cache()
{

}

bool Game::is_ending()
{
	int count = 0;
	for (int i = 0; i < 8; ++i) {
		const GamePlayer &player = players[i];
		if (player.hp > 0) {
			++count;
			if (count > 1) {
				break;
			}
		}
	}

	return count < 2;
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

		for (auto &it : player1->boardHeros) {
			it->round_obj = fight;
			fight->push_fight_unit(it, true);
		}

		for (auto &it : player2->boardHeros) {
			it->round_obj = fight;
			fight->push_fight_unit(it, false);
		}

		fight->is_mirror = mirror;
		fight->p1 = player1;
		fight->p2 = player2;
		fight->pre_enable_buffs();
		fight->random_units();

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
		for (int id : this->phase->monsters) {
			const pve_monster *monster_cfg  = CfgManager::get_instance().game_cfg.get_pve_monster(id);
			if (!monster_cfg) {
				continue;
			}

			const Hero *hero_cfg = CfgManager::get_instance().hero_cfg.get_hero(monster_cfg->monster_id);
			if (!hero_cfg) {
				continue;
			}

			HeroBase *unit = dynamic_cast<HeroBase *>(ObjectManager::get_instance().clone_one_clean_object<HeroBase>(hero_cfg->impl_name));
			if (!unit) {
				continue;
			}

			unit->id = this->get_gid();

			// TODO buff等数值
			for (auto &it : monster_cfg->buffs) {
				const Buff *buff_cfg =  CfgManager::get_instance().buff_cfg.get_buff(it.first);
				if (!buff_cfg) {
					continue;
				}

				BuffBase *buff = dynamic_cast<BuffBase *>(ObjectManager::get_instance().clone_one_clean_object<BuffBase>(buff_cfg->impl_name));
				if (!buff) {
					continue;
				}

				buff->params = it.second;
				unit->add_buff(buff);
			}

			if (monster_cfg->skill_id) {
				const Skill * skill_cfg = CfgManager::get_instance().skill_cfg.get_skill(monster_cfg->skill_id);
				if (skill_cfg) {
					SkillBase *skill = dynamic_cast<SkillBase *>(ObjectManager::get_instance().clone_one_clean_object<SkillBase>(skill_cfg->impl_name));
					if (skill) {
						unit->skill = skill;
					}
				}
			}

			unit->hero_cfg = hero_cfg;
			unit->star = monster_cfg->star;
			unit->pos = {(float)monster_cfg->position[0], (float)monster_cfg->position[1]};
			
			unit->round_obj = fight;
			fight->p2 = &player;
			fight->push_fight_unit(unit, false);
		}

		for (auto &it : player.boardHeros) {
			it->round_obj = fight;
			fight->push_fight_unit(it, true);
		}

		fight->pre_enable_buffs();
		fight->random_units();

		this->round_objects.push_back(fight);
	}
	
	on_fight = true;
}

int Game::get_frame_time()
{
	return (int)(!normal_rate ? FRAME(frame_rate) : FRAME(quick_frame_rate));
}

