#include "game/map.h"
#include "game/hero.h"

float GameMap::distance(FightUnit *unit)
{
    const HexCube &p1 = qoffset_to_cube(unit->pos);
    const HexCube &p2 = qoffset_to_cube(unit->enemy->pos);
    return cube_distance(p1, p2);;
}

std::vector<Vector2> GameMap::find_path(Vector2 from, Vector2 to)
{
    const std::vector<Node *> &path = aStarSearch(new Node(qoffset_to_cube(from)), new Node(qoffset_to_cube(to)));

    return {};
}

