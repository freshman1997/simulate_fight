#ifndef __EQUIP_H__
#define __EQUIP_H__
#include "actor.h"

class FightUnit;

class EquipBase : public Actor
{
public:
    virtual void update();

    // 进入战斗前计算
    virtual void on_begin() = 0;    

    virtual EquipBase * clone() = 0;
    
public:
    FightUnit *owner;
};

#endif
