# coding=utf-8

xls_config = {
    "buff": [
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "buff", "output": "buff.json", }
    ],
    
    "fight": [
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "misc", "output": "fight_misc.json", },
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "阶段", "output": "fight_phase.json", }
    ],
    "hero": [
        { "parse_file": "Y_英雄.xlsx", "parse_sheet": "英雄", "output": "hero.json", }
    ],
}