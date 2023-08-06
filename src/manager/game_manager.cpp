#include "manager/game_manager.h"
#include "game/game.h"

GameManager & GameManager::get_instance()
{
    static GameManager instance;
    return instance;
}

GameManager::GameManager() : id(0)
{}

int GameManager::get_id()
{
    return ++id;
}

bool GameManager::create_game(const std::vector<int> &players)
{
    // 检查玩家信息，是否在战斗中、被封号...
    if (players.size() != 8) {
        return false;
    }

    int id = get_id();
    if (this->games.count(id)) {
        return false;
    }

    std::shared_ptr<Game> game = std::make_shared<Game>();
    game->id = id;

    int i = 0;
    for (auto &it : players) {
        game->players[i].player_id = it;
        i++;
    }

    this->games[id] = game;

    return true;
}

bool GameManager::start_game(int id)
{
    auto it = this->games.find(id);
    if (it == this->games.end() || !it->second->start_game()) {
        return false;
    }

    return it->second->start_game();
}

std::shared_ptr<Game> GameManager::get_game(int id)
{
    auto it = this->games.find(id);
    if (it == this->games.end()) {
        return nullptr;
    }

    return it->second;
}

void GameManager::cancal_game(int id)
{
    auto it = this->games.find(id);
    if (it == this->games.end()) {
        return;
    }

    it->second->end_game();
}

void GameManager::pause_or_continue_game(int id)
{
    auto it = this->games.find(id);
    if (it == this->games.end()) {
        return;
    }

    it->second->pause = !it->second->pause;
}

