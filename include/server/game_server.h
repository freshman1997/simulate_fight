#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__
#include <unordered_map>
#include "timer/timer.h"

class GameServer
{
public:
    GameServer();
    bool init();
    void server_loop();

private:
    bool stop;
    Timer timer;
};

#endif
