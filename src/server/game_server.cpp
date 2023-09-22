#include "server/game_server.h"
#include "game/game.h"
#include <memory>

GameServer::GameServer() : stop(false)
{}

bool GameServer::init()
{
    return true;
}

void GameServer::server_loop()
{
    GamePlayer players[2];
    std::shared_ptr<Game> game = std::make_shared<Game>();
    game->timer = &this->timer;
    //game->start_game();
    game->test_game(GameState::idle_normal, 3);
    while (!stop) {
        // net loop
        // timer
        timer.CheckTimer();
    }
}
