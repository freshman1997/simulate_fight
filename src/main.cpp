
#include <iostream>
#include <vector>
#include <map>
#include <ctime>

#include "timer/heap.h"
#include "timer/timer.h"
#include "manager/cfg_manager.h"
#include "server/game_server.h"

using namespace std;

vector<pair<int, vector<pair<int, int>>>> result;

void dfs(int x, int y, int n, int m, int &count, vector<pair<int, int>> tmp)
{
    if (x > n || y > m) return;

    tmp.push_back({x, y});
    if (x == n && y == m) {
        result.push_back({count + 1, tmp});
        ++count;
        return;
    }

    dfs(x + 1, y, n, m, count, tmp);
    dfs(x, y + 1, n, m, count, tmp);
}

int path(int n, int m)
{
    int res = 1;
    dfs(0, 0, n, m, res, {});
    for (auto &it : result) {
        cout << "cur: " << it.first << endl;
        int i  = 0;
        for (auto &it1 : it.second) {
            cout << "(" << it1.first << ", " << it1.second << ")";
            if (i > 0 && i % 2 == 0) cout << '\n';
            ++i;
        }
        cout << '\n';
    }
    return 0;
}

int wb(int n)
{
    int w[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int v[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int res = 0;

    return res;
}

int operator "" _ms1(unsigned long long a)
{
    return a * 1000;
}

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
    //GameServer server;
    //server.init();

    //path(3, 3);
    //Test_AStar();
    return 0;
}