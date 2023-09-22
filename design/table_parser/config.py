# coding=utf-8

xls_config = {
    "buff": [
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "buff", "output": "buff.json", },
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "常规强化符文", "output": "normal_power_buff.json", },
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "英雄传说之力符文", "output": "hero_power_buff.json", },
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "英雄传说之力", "output": "hero_power_detail.json", },
        { "parse_file": "B_Buff.xlsx", "parse_sheet": "海克斯buff", "output": "hekes_buff.json", }
    ],
    
    "fight": [
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "misc", "output": "fight_misc.json", },
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "阶段", "output": "fight_phase.json", },
        { "parse_file": "Z_战斗.xlsx", "parse_sheet": "野怪", "output": "pve_monster.json", }
    ],
    
    "hero": [
        { "parse_file": "Y_英雄.xlsx", "parse_sheet": "英雄", "output": "hero.json", },
        { "parse_file": "Y_英雄.xlsx", "parse_sheet": "羁绊", "output": "hero_fetter.json", },
    ],
    
    "skill": [
        { "parse_file": "J_技能.xlsx", "parse_sheet": "技能", "output": "skill.json", },
        { "parse_file": "J_技能.xlsx", "parse_sheet": "被动技能", "output": "passive_skill.json", },
        { "parse_file": "J_技能.xlsx", "parse_sheet": "瑞兹技能", "output": "riz_skill.json", }
    ],
    
    "player": [
        { "parse_file": "W_玩家相关.xlsx", "parse_sheet": "misc", "output": "player_misc.json", }
    ],
    
    "level": [
        { "parse_file": "D_等级.xlsx", "parse_sheet": "等级", "output": "level.json", },
        { "parse_file": "D_等级.xlsx", "parse_sheet": "星级", "output": "star.json", },
    ],
    
    "scene": [
        { "parse_file": "Z_阵营场景.xlsx", "parse_sheet": "阵营", "output": "camp.json", },
        { "parse_file": "Z_阵营场景.xlsx", "parse_sheet": "场景", "output": "scene.json", },
    ],
    
    "equips": [
        { "parse_file": "Z_装备.xlsx", "parse_sheet": "装备", "output": "equips.json", },
        { "parse_file": "Z_装备.xlsx", "parse_sheet": "辅助装备", "output": "aux_equips.json", },
        { "parse_file": "Z_装备.xlsx", "parse_sheet": "羁绊装备", "output": "fetter_equips.json", },
        { "parse_file": "Z_装备.xlsx", "parse_sheet": "奥恩神器", "output": "aoen_equips.json", },
        { "parse_file": "Z_装备.xlsx", "parse_sheet": "其他装备", "output": "other_equips.json", }
    ],
}