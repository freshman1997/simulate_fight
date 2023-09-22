#ifndef __KNOXUS_H__
#define __KNOXUS_H__

#include "../fetter.h"
#include "game/hero.h"

class Knoxus : public FetterBase
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();
    virtual void update(float delta);

    // 正式开启战斗之前
    virtual void on_before_round();
    
    void on_unit_die(FightUnit *killer, FightUnit *unit);
};

#endif