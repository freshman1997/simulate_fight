#include "game/fight.h"
#include "game/buff.h"
#include "game/event/event.h"
#include "game/map.h"
#include "game/game.h"
#include <memory>
#include <string.h>

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

Fight::Fight(Game *_game) : error(false), win_side(-1), rest_heros(0), map(std::make_shared<GameMap>()), game(_game), is_mirror(false)
{
    memset(die_counter, 0, sizeof(die_counter));
}

void Fight::clear_cache()
{
    
}

void Fight::update(float deltaTime)
{
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
            if (it->is_sommon) {
                ++sommon_amount;
            } else {
                ++count;
            }
        }
    }

    if (p1 && !is_mirror) {
        p1->on_fight_end(win_side == 0, count);
    }

    if (p2) {
        p2->on_fight_end(win_side == 1, count);
    }
}

// 进入战斗前激活已存在的buff，如海克斯、羁绊、装备的加成等
bool Fight::init()
{
    random_units();
    
    for (auto &it : fight_actors) {
        // 激活
        it->round_obj = this;
        if (it->init()) {
            return false;
        }
    }

    this->game->ev_manager->register_event(EventType::UNIT_DIE, this->id, [this](const EventParams &params) {
        if (win_side >= 0) {
            return;
        }

        FightUnit *unit = (FightUnit *)params.ptr;
        if (!unit || !unit->owner || (*unit->round_obj != *this) || (unit->owner->player_id != p1->player_id || unit->owner->player_id != p2->player_id)) {
            return;
        }

        if (unit->owner->player_id == p1->player_id) {
            ++die_counter[0];
            if (die_counter[0] >= player1.size()) {
                on_fight_end();
            }
        } else {
            ++die_counter[1];
            if (die_counter[1] >= player1.size()) {
                on_fight_end();
            }
        }
    });

    return true;
}

void Fight::deinit()
{
    
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

