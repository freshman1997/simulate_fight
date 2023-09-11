# coding=utf-8

xls_config = {
    "buff": [
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "buff", "output": "buff.json", },
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "非战斗buff", "output": "not_fighting_buff.json", }
    ],
    
    "fight": [
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "misc", "output": "fight_misc.json", },
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "阶段", "output": "fight_phase.json", },
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "野怪", "output": "pve_monster.json", }
    ],
    
    "hero": [
        { "parse_file": "Y_英雄.xlsx", "parse_sheet": "英雄", "output": "hero.json", },
        { "parse_file": "Y_英雄.xlsx", "parse_sheet": "职业羁绊", "output": "hero_fetters.json", }
    ],
    
    "skill": [
        { "parse_file": "J_技能.xlsx", "parse_sheet": "技能", "output": "skill.json", }
    ],
    
    "player": [
        { "parse_file": "W_玩家相关.xlsx", "parse_sheet": "misc", "output": "player_misc.json", }
    ],
    
    "level": [
        { "parse_file": "D_等级.xlsx", "parse_sheet": "等级", "output": "level.json", },
        { "parse_file": "D_等级.xlsx", "parse_sheet": "星级", "output": "star.json", },
    ],
}