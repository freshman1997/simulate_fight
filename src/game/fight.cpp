#include "game/fight.h"

Fight::Fight() : started(false), error(false), win_side(-1), rest_heros(0)
{}

void Fight::update(float deltaTime)
{
    if (!started) {
        pre_enable_buffs();
        random_units();
        started = true;
    }

    for (auto &it : fight_actors) {
        it->update(deltaTime);
    }
}

void Fight::push_fight_unit(FightUnit *unit, bool atk)
{
    if (atk) {
        unit->side = 1;
        player1.push_back(unit);
    } else {
        unit->side = 0;
        player2.push_back(unit);
    }
}


// 进入战斗前激活已存在的buff，如海克斯、羁绊、装备的加成等
void Fight::pre_enable_buffs()
{
    for (auto &it : fight_actors) {
        // 激活

    }
}

void Fight::random_units()
{
    size_t sz = player1.size() + player2.size();

    fight_actors.clear();

    std::vector<FightUnit *> tmp;
    tmp.reserve(sz);
    std::copy(player1.begin(), player1.end(), tmp.begin());
    std::copy(player2.begin(), player2.end(), tmp.begin());

    for (size_t i = 0; i < sz; ++i) {
        fight_actors.push_back(tmp[rand() % sz]);
    }
}

bool Fight::is_end()
{
    if (win_side >= 0) return true;

    int count1 = 0, count2 = 0;
    for (auto &it : fight_actors) {
        if (it->side && it->is_die()) {
            ++count1;
        } else {
            count2 = it->is_die() ? count2 + 1 : count2;
        }
    }

    if (count1 == player1.size()) {
        this->win_side = 0;
    }

    if (count2 == player2.size()) {
        this->win_side = 1;
    }

    return win_side >= 0;
}
