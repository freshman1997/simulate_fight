#ifndef __MAP_H__
#define __MAP_H__
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>
#include "actor.h"
#include "game/actor.h"

typedef Vector2 OffsetCoord;
class FightUnit;

struct HexCube
{
	float q;
	float r;
	float s;
	HexCube() : q(0), r(0), s(0) {}
	HexCube(float _q, float _r, float _s) : q(_q), r(_r), s(_s) {}

	bool operator == (const HexCube& vec3)
	{
		return this->q == vec3.q && this->r == vec3.r && this->s == vec3.s;
	}

	bool operator != (const HexCube& vec3)
	{
		return this->q != vec3.q || this->r != vec3.r || this->s != vec3.s;
	}
};

struct Node {
    HexCube cude;  // 节点坐标
    int g, h;  // g代表起点到当前节点的实际代价，h代表当前节点到目标节点的估计代价
    Node* parent;  // 指向父节点的指针

    Node(const HexCube &cube) {
        this->cude = cube;
        g = 0;
        h = 0;
        parent = nullptr;
    }

    int f() const {  // f = g + h
        return g + h;
    }
};

class GameMap
{
public:
    GameMap()
    {
        hex_dir[0][0] = 1;
		hex_dir[0][1] = 0;
		hex_dir[0][2] = -1;

		hex_dir[1][0] = 1;
		hex_dir[1][1] = -1;
		hex_dir[1][2] = 0;

		hex_dir[2][0] = 0;
		hex_dir[2][1] = -1;
		hex_dir[2][2] = 1;

		hex_dir[3][0] = -1;
		hex_dir[3][1] = 0;
		hex_dir[3][2] = 1;

		hex_dir[4][0] = -1;
		hex_dir[4][1] = 1;
		hex_dir[4][2] = 0;

		hex_dir[5][0] = 0;
		hex_dir[5][1] = 1;
		hex_dir[5][2] = -1;

		for (int i = 0; i < row; ++i) {
			game_map.push_back({});
			for (int j = 0; j < col; ++j) {
				game_map[i].push_back({});
				MapHolder &holder = game_map[i][j];
				holder.pos = qoffset_to_cube({(float)i, (float)j});
			}
		}
    }

	float distance(const FightUnit *unit);

	float distance(const FightUnit *unit1, const FightUnit *unit2);

	// 范围内所有
    std::vector<FightUnit *> find_around(int len, const OffsetCoord &pos);

	// 范围内随机一个
    FightUnit * rand_around_one(int len, const OffsetCoord &pos);

	// 与目标单位方向上所有
	std::vector<FightUnit *> dir_units(const FightUnit *unit);

	void find_path(Vector2 from, Vector2 to, std::vector<Vector2> &, float);

	bool move(FightUnit *unit, Vector2 pos);

	void set_hero(FightUnit *unit, Vector2 pos);

	void remove(FightUnit *unit);

	bool is_valid_pos(Vector2 pos);

	bool has_unit(const HexCube& cube)
	{
		OffsetCoord offc = qoffset_from_cube(cube);
		if (!is_valid_pos(offc)) return false;
		return game_map[offc.x][offc.y].units.empty();
	}

	bool has_unit(const OffsetCoord& offc)
	{
		if (!is_valid_pos(offc)) return false;
		return game_map[offc.x][offc.y].units.empty();
	}

	bool is_overbound(const HexCube& cube)
	{
		OffsetCoord offc = qoffset_from_cube(cube);
		return offc.x < 0 || offc.x >= row || offc.y < 0 || offc.y > col;
	}

	float vector2_distance(Vector2 pos1, Vector2 pos2)
	{
		return hex_distance(qoffset_to_cube(pos1), qoffset_to_cube(pos2));
	}


private:
    struct MapHolder;

	// 斜边长度
	float hex_diagonallength(const HexCube& cube)
	{
		return sqrt(std::abs(cube.q) * std::abs(cube.q) + std::abs(cube.r) * std::abs(cube.r) + std::abs(cube.s) * std::abs(cube.s));
	}

	float hex_length(const HexCube& cube)
	{
		// 向下取整
		return (std::abs(cube.q) + std::abs(cube.r) + std::abs(cube.s)) / 2;
	}

	HexCube hex_subtract(const HexCube& a, const HexCube& b)
	{
		return HexCube(a.q - b.q, a.r - b.r, a.s - b.s);
	}

	float hex_distance(const HexCube& a, const HexCube& b)
	{
		return hex_length(hex_subtract(a, b));
	}

	// 斜边长度
	float hex_diagonaldistance(const HexCube& a, const HexCube& b)
	{
		return hex_diagonallength(hex_subtract(a, b));
	}

	HexCube hex_add(const HexCube& a, const HexCube& b)
	{
		return HexCube(a.q + b.q, a.r + b.r, a.s + b.s);
	}

	HexCube hex_direction(int direction)
	{
		return { hex_dir[direction][0], hex_dir[direction][2] , hex_dir[direction][2] };
	}

	HexCube hex_neighbor(const HexCube& cube, int direction)
	{
		return hex_add(cube, hex_direction(direction));
	}

	float cube_distance(MapHolder& p1, MapHolder& p2)
	{
		return std::abs(p1.pos.q - p2.pos.q) + std::abs(p1.pos.r - p2.pos.r) + std::abs(p1.pos.s - p2.pos.s);
	}

	float cube_distance(const HexCube& a, const HexCube &b)
	{
		return std::abs(a.q - b.q) + std::abs(a.r - b.r) + std::abs(a.s - b.s);
	}

	// convert odd - q offset to cube
	HexCube qoffset_to_cube(const OffsetCoord& offCoord)
	{
		float q = offCoord.y;
		float r = offCoord.x - (offCoord.y - (int(offCoord.y) & 1)) / 2;
		float s = -q - r;
		return HexCube(q, r, s);
	}

	OffsetCoord qoffset_from_cube(const HexCube& h)
	{
		float col = h.q;
		float row = h.r + (h.q - (int(h.q) & 1)) / 2;
		return { col, row };
	}

	HexCube roffset_to_cube(const OffsetCoord& offCoord)
	{
		float q = offCoord.y - (offCoord.x - (int(offCoord.x) & 1)) / 2;
		float r = offCoord.x;
		float s = -q - r;
		return HexCube(q, r, s);
	}

	OffsetCoord roffset_from_cube(const HexCube& h)
	{
		float col = h.q + (h.r - (int(h.r) & 1)) / 2;
		float row = h.r;
		return { col, row };
	}

	float heuristic(const HexCube& a, const HexCube &b)
	{
		return hex_diagonaldistance(a, b);
	}

	HexCube hex_round(const HexCube& h)
	{
		float q = round(h.q);
		float r = round(h.r);
		float s = round(h.s);
		float q_diff = std::abs(q - h.q);
		float r_diff = std::abs(r - h.r);
		float s_diff = std::abs(s - h.s);
		if (q_diff > r_diff && q_diff > s_diff) {
			q = -r - s;
		}
		else {
			if (r_diff > s_diff) {
				r = -q - s;
			}
			else {
				s = -q - r;
			}
		}

		return HexCube(q, r, s);
	}

	HexCube hex_lerp(const HexCube &a, const HexCube &b, float t)
	{
		return HexCube(a.q * (1 - t) + b.q * t, a.r * (1 - t) + b.r * t, a.s * (1 - t) + b.s * t);
	}

	// 没有障碍物的路线
	std::vector<HexCube> hex_linedraw(const HexCube &a, const HexCube &b)
	{
		float N = hex_distance(a, b);
		HexCube a_nudge = HexCube(a.q + 0.000001f, a.r + 0.000001f, a.s - 0.000002f);
		HexCube b_nudge = HexCube(b.q + 0.000001f, b.r + 0.000001f, b.s - 0.000002f);
		
		std::vector<HexCube> res;
		float step = 1.0f / std::max(N, 1.0f);
		int amount = int(N) + 1;
		for (int i = 0; i < amount; ++i) {
			res.push_back(hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
		}

		return res;
	}

private:
	// 计算节点间的曼哈顿距离
	float calculateDistance(const Node& a, const Node& b) {
		return heuristic(a.cude, b.cude);
	}

	// 检查节点是否在开放列表中
	bool isInOpenList(const std::vector<Node*>& openList, const Node* node) {
		for (const auto& n : openList) {
			if (n->cude == node->cude) {
				return true;
			}
		}
		return false;
	}

	// 检查节点是否在关闭列表中
	bool isInClosedList(const std::vector<Node*>& closedList, const Node* node) {
		for (const auto& n : closedList) {
			if (n->cude == node->cude) {
				return true;
			}
		}
		return false;
	}

	// 从起点到目标节点的路径
	std::vector<Node*> getPath(Node* node) {
		std::vector<Node*> path;
		while (node != nullptr) {
			path.push_back(node);
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
		return path;
	}

	// A*算法实现
	std::vector<Node*> aStarSearch(Node* start, Node* target, float);

public:
	

private:
    float hex_dir[6][3];
    struct MapHolder
    {
		// 包括英雄，召唤物，技能产生的实体等
		std::unordered_map<int, FightUnit *> units;
        HexCube pos;
    };
    
    // 行列
    int row = 7;
    int col = 8;

	std::vector<std::vector<MapHolder>> game_map;
	std::vector<FightUnit *> map_objects;
};

#endif