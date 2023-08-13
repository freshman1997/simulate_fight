#include "game/skill.h"
#include "game/fight.h"
#include "game/game.h"
#include "game/ai.h"

void SkillBase::update(float deltaTime)
{
    // 这个标志由外部修改
    if (is_stuck) {
        start_time = 0;
        start_skill = false;
        return;
    }

    if (start_singing > 0) {
        singing_time += deltaTime;
        if (singing_time >= skill_time) {
            start_skill = true;
        }
    }

    if (start_skill) {
        if (start_singing) {
            start_time += singing_time - skill_time;
            start_singing = false;
            singing_time = 0;
            // TODO 输出到队列
        } else {
            start_time += deltaTime;
        }

        if (start_time >= skill_time) {
            // TODO 造成伤害，输出到队列
            perform_skill();
            start_time = 0;
            start_skill = false;
        }
    }
}

void SkillBase::perform_skill()
{
    if (!start_skill) return;

    
}

void SkillBase::break_skill()
{   
    if (!start_singing || !start_skill) return;

    is_stuck = true;
    // TODO
}

void SkillBase::continue_skill()
{
    if (!start_singing || !start_skill) return;

    is_stuck = false;
    // TODO
}

bool SkillBase::is_performing()
{
    return start_time > 0;
}

