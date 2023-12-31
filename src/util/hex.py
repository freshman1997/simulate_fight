﻿# -*- coding: utf-8 -*-

import math
import collections

HexCube = collections.namedtuple("HexCube", ["q", "r", "s"])
OffsetCoord = collections.namedtuple("OffsetCoord", ["col", "row"])

def hex_diagonallength(hexcube):
    return math.sqrt(abs(hexcube.q)*abs(hexcube.q) + abs(hexcube.r)*abs(hexcube.r) + abs(hexcube.s)*abs(hexcube.s))

def hex_length(hexcube):
    return (abs(hexcube.q) + abs(hexcube.r) + abs(hexcube.s)) // 2

def hex_subtract(a, b):
    return HexCube(a.q - b.q, a.r - b.r, a.s - b.s)

def hex_distance(a, b):
    return hex_length(hex_subtract(a, b))

def hex_diagonaldistance(a, b):
    return hex_diagonallength(hex_subtract(a, b))

def hex_neighbor(hexcube, direction):
    return hex_add(hexcube, hex_direction(direction))

def hex_add(a, b):
    return HexCube(a.q + b.q, a.r + b.r, a.s + b.s)

hex_directions = [HexCube(1, 0, -1), HexCube(1, -1, 0), HexCube(0, -1, 1), HexCube(-1, 0, 1), HexCube(-1, 1, 0), HexCube(0, 1, -1)]

def hex_direction(direction):
    return hex_directions[direction]

#convert odd-q offset to cube
def qoffset_to_cube(h):
    q = h.col
    r = h.row - (h.col - (h.col & 1)) // 2
    s = -q - r
    return HexCube(q, r, s)

def qoffset_from_cube(h):
    col = h.q
    row = h.r + (h.q - (h.q & 1)) // 2
    return OffsetCoord(col, row)

def roffset_to_cube(h):
    q = h.col - (h.row -  (h.row & 1)) // 2
    r = h.row
    s = -q - r
    return HexCube(q, r, s)

def roffset_from_cube(h):
    col = h.q + (h.r -  (h.r & 1)) // 2
    row = h.r
    return OffsetCoord(col, row)

def heuristic(a, b):
    return hex_diagonaldistance(a, b)  # 斜对角线距离

def hex_round(h):
    q = int(round(h.q))
    r = int(round(h.r))
    s = int(round(h.s))
    q_diff = abs(q - h.q)
    r_diff = abs(r - h.r)
    s_diff = abs(s - h.s)
    if q_diff > r_diff and q_diff > s_diff:
        q = -r - s
    else:
        if r_diff > s_diff:
            r = -q - s
        else:
            s = -q - r
    return HexCube(q, r, s)

def hex_lerp(a, b, t):
    return HexCube(a.q * (1 - t) + b.q * t, a.r * (1 - t) + b.r * t, a.s * (1 - t) + b.s * t)

def hex_linedraw(a, b):
    N = hex_distance(a, b)
    a_nudge = HexCube(a.q + 0.000001, a.r + 0.000001, a.s - 0.000002)
    b_nudge = HexCube(b.q + 0.000001, b.r + 0.000001, b.s - 0.000002)
    results = []
    step = 1.0 / max(N, 1)
    for i in range(0, N + 1):
        results.append(hex_round(hex_lerp(a_nudge, b_nudge, step * i)))
    return results

if __name__=="__main__":
    first=HexCube(0,0,0)
    second=HexCube(2,3,-5)
    print (hex_linedraw(first,second))