#ifndef __GAME_SOCKET_H__
#define __GAME_SOCKET_H__
#include <string>

class Socket
{
public:
    Socket(std::string ip, int port);
    
private:
    std::string ip;
    int port;
};

#endif
