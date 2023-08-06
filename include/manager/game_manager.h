#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__
#include <vector>
#include <unordered_map>
#include <memory>

class Game;

class GameManager
{
public:
    static GameManager & get_instance();
    bool create_game(const std::vector<int> &players);
    bool start_game(int id);
    std::shared_ptr<Game> get_game(int id);
    void cancal_game(int id);
    void pause_or_continue_game(int id);

private:
    GameManager();
    int get_id();
    int id;
    std::unordered_map<int, std::shared_ptr<Game>> games;
};

#endif