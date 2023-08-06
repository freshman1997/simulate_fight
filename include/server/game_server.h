#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

class GameServer
{
public:
    GameServer();
    bool init();
    void server_loop();

private:
    bool stop;

};

#endif
