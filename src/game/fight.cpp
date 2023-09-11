#include "game/fight.h"
#include "game/map.h"
#include "game/game.h"

Object * Fight::clone()
{
    return new Fight(game);
}

Object * Fight::clone_and_clean()
{
    Fight *f = dynamic_cast<Fight *>(clone());
    f->clear_cache();
    return f;
}

Fight::Fight(Game *_game) : started(false), error(false), win_side(-1), rest_heros(0), map(new GameMap), game(_game), is_mirror(false)
{}

void Fight::clear_cache()
{

}

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

void Fight::on_fight_end()
{
    if (win_side < 0) {
        return;
    }

    const std::vector<FightUnit *> &rest = win_side == 1 ? player2 : player1;
    int sommon_amount = 0, count = 0;
    for (auto &it : rest) {
        if (!it->is_die()) {
            ++count;
        }
    }

    if (p1 && !is_mirror) {
        p1->on_end_fight(win_side == 0, count);
    }

    if (p2) {
        p2->on_end_fight(win_side == 1, count);
    }
}

// 进入战斗前激活已存在的buff，如海克斯、羁绊、装备的加成等
void Fight::pre_enable_buffs()
{
    for (auto &it : fight_actors) {
        // 激活
        it->init();
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
