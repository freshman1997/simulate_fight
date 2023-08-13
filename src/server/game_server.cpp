#include "server/game_server.h"
#include "game/game.h"

GameServer::GameServer() : stop(false)
{}

bool GameServer::init()
{
    return true;
}

void GameServer::server_loop()
{
    Game game;
    game.timer = &this->timer;
    game.start_game();
    while (!stop) {
        // net loop
        // timer
        timer.CheckTimer();
    }
}