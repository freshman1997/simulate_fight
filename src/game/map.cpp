#include "game/map.h"
#include "game/hero.h"

float GameMap::distance(FightUnit *unit)
{
    const HexCube &p1 = qoffset_to_cube(unit->pos);
    const HexCube &p2 = qoffset_to_cube(unit->enemy->pos);
    return hex_distance(p1, p2);;
}

