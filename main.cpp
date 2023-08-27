
#include <iostream>
#include <vector>
#include <map>
#include <ctime>

#include "timer/heap.h"
#include "timer/timer.h"
#include "manager/cfg_manager.h"
#include "server/game_server.h"
#include "script/lua_adapter.h"
#include "game/map.h"

using namespace std;

static bool pre_start()
{
    CfgManager &cfgMgr = CfgManager::get_instance();
    cfgMgr.set_config_path("D:/code/src/vs/simulate_fight/design/jsons/");
    if (!cfgMgr.load_cfg()) {
        cout << "load config fail!!\n";
        return false;
    }

    return true;
}

extern void Test_AStar();

int main()
{
    GameMap m;
    m.find_path({0, 0}, {1, 1});
    LuaInterpreter li;
    li.excute_file("../lua_source/test.lua");

    if (!pre_start()) {
        return -1;
    }
    
    GameServer server;
    if (!server.init()) {
        return -1;
    }

    server.server_loop();
    return 0;
}