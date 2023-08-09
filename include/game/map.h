#ifndef __MAP_H__
#define __MAP_H__
#include <vector>
#include <cmath>
#include "actor.h"

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
    }

    std::vector<FightUnit *> find_around(int len, const OffsetCoord &pos);
    FightUnit * rand_around_one(int len, const OffsetCoord &pos);

	float distance(FightUnit *unit);

private:

    class MapHolder;

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
		return std::abs(p1.pos.q = p2.pos.q) + std::abs(p1.pos.r = p2.pos.r) + std::abs(p1.pos.s = p2.pos.s);
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
    float hex_dir[6][3];
    struct MapHolder
    {
        FightUnit *unit;
        HexCube pos;
    };
    
    // 行列
    int row = 7;
    int col = 8;
};

#endif