#include "game/map.h"
#include "game/hero.h"

float GameMap::distance(const FightUnit *unit)
{
    const HexCube &p1 = qoffset_to_cube(unit->pos);
    const HexCube &p2 = qoffset_to_cube(unit->enemy->pos);
    return cube_distance(p1, p2);
}

float GameMap::distance(const FightUnit *unit1, const FightUnit *unit2)
{
    const HexCube &p1 = qoffset_to_cube(unit1->pos);
    const HexCube &p2 = qoffset_to_cube(unit2->pos);
    return cube_distance(p1, p2);
}

std::vector<FightUnit *> GameMap::find_around(int len, const OffsetCoord &pos)
{
    return {};
}

FightUnit * GameMap::rand_around_one(int len, const OffsetCoord &pos)
{
    return nullptr;
}

std::vector<FightUnit *> GameMap::dir_units(const FightUnit *unit)
{
    return {};
}

static std::vector<Vector2> GetCoverCoordinates(FightUnit *unit, Vector2 pos)
{
    std::vector<Vector2> res;

    for (int i = 0; i < unit->hero_cfg->cover_size[0]; ++i) {
        for (int j = 0; j < unit->hero_cfg->cover_size[1]; ++j) {
            res.push_back({pos.x + i, pos.y + j});
        }
    }

    return res;
}

bool GameMap::is_valid_pos(Vector2 pos)
{
    return pos.x >= 0 && pos.x < col && pos.y >=0 && pos.y < row;
}

bool GameMap::move(FightUnit *unit, Vector2 pos)
{
    const std::vector<Vector2> &positions = GetCoverCoordinates(unit, pos);
    for (auto &it : positions) {
        if (!is_valid_pos(it)) {
            return false;
        }
    }

    this->remove(unit);

    for (auto &it : positions) {
        this->game_map[it.x][it.y].units[unit->id] = unit;
    }

    return true;
}

void GameMap::set_hero(FightUnit *unit, Vector2 pos)
{
    if (!unit || !is_valid_pos(pos)) {
        return;
    }

    this->game_map[pos.x][pos.y].units[unit->id] = unit;
}

void GameMap::remove(FightUnit *unit)
{
    auto it = this->game_map[unit->pos.x][unit->pos.y].units.find(unit->id);
    if (!unit || !is_valid_pos(unit->pos) || it == this->game_map[unit->pos.x][unit->pos.y].units.end()) {
        return;
    }

    this->game_map[unit->pos.x][unit->pos.y].units.erase(unit->id);
}

std::vector<Node*> GameMap::aStarSearch(Node* start, Node* target)
{
    if (!start || !target) {
        return {};
    }

    std::vector<Node *> openList;  // 开放列表，存放待探索的节点
    std::vector<Node *> closedList;  // 关闭列表，存放已探索的节点

    openList.push_back(start);

    while (!openList.empty()) {
        // 在开放列表中查找f值最小的节点
        Node *currentNode = openList[0];
        int currentIndex = 0;
        for (int i = 1; i < openList.size(); ++i) {
            if (openList[i]->f() < currentNode->f()) {
                currentNode = openList[i];
                currentIndex = i;
            }
        }

        // 从开放列表中移除当前节点，加入关闭列表
        openList.erase(openList.begin() + currentIndex);
        closedList.push_back(currentNode);

        // 找到目标节点，返回路径
        if (currentNode->cude == target->cude) {
            return getPath(currentNode);
        }

        // 遍历当前节点的邻居节点
        std::vector<Node *> neighbors;
        
        // 在实际应用中，此处根据地图数据获取或计算邻居节点
        // 这里简化为上下左右四个方向
        //int dx[4] = {0, 0, -1, 1};
        //int dy[4] = {-1, 1, 0, 0};
        for (int i = 0; i < 6; ++i) {
            float *dir = hex_dir[i];
            float newX = currentNode->cude.q + dir[0];
            float newY = currentNode->cude.r + dir[1];
            float newZ = currentNode->cude.s + dir[2];

            Node nd({newX, newY, newZ});

            // nd 是否有障碍物
            // 跳过越界或在关闭列表中的节点
            if (is_overbound(nd.cude) || has_unit(nd.cude) || isInClosedList(closedList, &nd)) {
                continue;
            }

            Node* neighbor = new Node({newX, newY, newZ});
            neighbor->parent = currentNode;
            neighbor->g = currentNode->g + 1;
            neighbor->h = calculateDistance(*neighbor, *target);

            // 如果邻居节点已经在开放列表中，检查是否有更优的路径
            if (isInOpenList(openList, neighbor)) {
                for (const auto& n : openList) {
                    if (n->cude == neighbor->cude) {
                        if (neighbor->g < n->g) {
                            n->g = neighbor->g;
                            n->parent = neighbor->parent;
                        }
                        break;
                    }
                }

                delete neighbor;
            } else {
                openList.push_back(neighbor);
            }
        }
    }

    return {};  // 无法找到路径，返回空路径
}

void GameMap::find_path(Vector2 from, Vector2 to, std::vector<Vector2> &res)
{
    const std::vector<Node *> &path = aStarSearch(new Node(qoffset_to_cube(from)), new Node(qoffset_to_cube(to)));
    if (path.empty()) {
        return;
    }

    for (auto &it : path) {
        res.push_back({ roffset_from_cube(it->cude) });
        delete it;
    }
}
