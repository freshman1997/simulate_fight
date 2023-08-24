#include <thread>
#include <chrono>
#include "game/game.h"
#include "game/fight.h"
#include "timer/timer.h"
#include "manager/cfg_manager.h"

GamePlayer::GamePlayer() : player_id(0), hp(0), gold(0), win_amount(0), defeat_amount(0), interest(0)
{}

bool GamePlayer::use_gold(int amount)
{
	return true;
}

void GamePlayer::defeat(int rest_heros)
{

}

//----------------------- game -------------------------//
Game::Game() : round(0), pause(false), start_time(0), state(GameState::none), state_start_time(0), on_fight(false), normal_rate(false), next_time(0)
{
	const CfgManager &mgr = CfgManager::get_instance();
	frame_rate = mgr.game_cfg.get_frame_rate();
	quick_frame_rate = mgr.game_cfg.get_quick_frame_rate();
	cur_frame_time = (int)FRAME(frame_rate);
	round_end_count = 0;
	state_pass_time = 0;
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
}

void Game::continue_game()
{
	pause = false;
	resume = true;
	game_loop();
}

void Game::game_loop()
{
	if (pause || end_time != 0) {
		return;
	}

	GameState last_state = state;
	long long now = 100;
	if (resume) {

	}

	int next_time = 0;	// 下次执行的时间，修改帧率会影响到这里

	switch (state)
	{
	case GameState::select_scene: {
		if (now - state_start_time < 10) {
			
		} else {
			// 根据当前阶段产生选秀的英雄和装备
			state = GameState::select_weapon_with_hero;
			
		}
		break;
	}
	case GameState::select_weapon_with_hero: {
		if (now - state_start_time < 10) {
			
		}
		// 根据配置决定是 pvp 还是 pve
		// 产生随机1费卡给每个玩家

		break;
	}
	case GameState::idle_normal: {
		if (now - state_start_time < 10) {
			
		}

		break;
	}
	case GameState::idle_spec: {
		if (now - state_start_time < 10) {
			// 这里不应该跑到

		} 

		// 切换状态，准备战斗数据
		break;
	}
	case GameState::fight: {
		if (now - state_start_time < 10) {
			// 双方血量大于 0，则正常跑，一方为0则直接胜利，对方已投降
			next_time = 100;
		} else {
			if (!is_all_fight_end()) {
				normal_rate = true;
				next_time = get_frame_time();
			} else {

			}
		}
		break;
	}

	default:
		// TODO
		return;
	}

	this->next_time = next_time;
	
	if (last_state != state) {
		// state change, 结算利息
		state_start_time = 100;
		++round;

	}

	if (state == GameState::fight) {
		bool result = this->update((float)FRAME(frame_rate));
		if (!result && !pause) {
			end_game();
			return;
		}
	}

	timer->AddTimer(next_time, [this](const TimerNode &node){
    	game_loop();
    });
}

void Game::clear_cache()
{

}

bool Game::start_game()
{
	// TODO 丢到定时器
	start_time = 0;
	state_start_time = 100;
	state = GameState::select_scene;
	// 产生随机阵容
	// 计算时间到第一回合
	next_time = 1000 * 30;
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
			it->update(delta);
		}

		return true;
	} catch(...) {
		return false;
	}
}

void Game::start_pvp()
{

	on_fight = true;
}

void Game::start_pve()
{

	on_fight = true;
}

int Game::get_frame_time()
{
	return (int)(!normal_rate ? FRAME(frame_rate) : FRAME(quick_frame_rate));
}

