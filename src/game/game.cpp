#include <thread>
#include <chrono>
#include "game/game.h"
#include "game/game_context.h"
#include "game/fight.h"

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
Game::Game() : round(0), pause(false), start_time(0), state(GameState::none), ctx(nullptr), state_start_time(0), on_fight(false)
{}

void Game::game_loop()
{
	if (pause || end_time != 0) {
		return;
	}

	GameState last_state = state;
	long long now = 100;
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
			// 结算当前回合

		}
		break;
	}

	default:
		// TODO
		return;
	}

	if (last_state != state) {
		// state change, 结算利息
		state_start_time = 100;
		++round;

	}

	if (state == GameState::fight) {
		bool result = this->update(next_time);
		if (!result && !pause) {
			end_game();
			return;
		}
	}

	// TODO 丢到定时器
}

int Game::get_next_time()
{
	// TODO 根据帧率计算下一帧执行时间
	last_time = 100;
	return 100;
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
	return true;
}

void Game::end_game()
{
	GameContext &gCtx = GameContext::get_game_context();
	gCtx.remove_context(std::this_thread::get_id());
	// TODO 
	end_time = 0;
}

bool Game::update(float delta)
{
	if (pause) {
		return false;
	}

	if (!this->ctx) {
		FightContext* context = GameContext::get_game_context().find_context(std::this_thread::get_id());
		if (!context) {
			return false;
		}

		this->ctx = context;
	}

	this->ctx->deltaTime = int(delta);
	try {
		this->ctx->on_frame();
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
