#include "server/game_server.h"

GameServer::GameServer() : stop(false)
{}

bool GameServer::init()
{
    return false;
}

void GameServer::server_loop()
{
    while (!stop) {
        // net loop
        // timer
    }
}