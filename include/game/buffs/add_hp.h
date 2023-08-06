#ifndef __ADD_HP_H__
#define __ADD_HP_H__

#include "../buff.h"

class AddHpBuff : BaseBuff
{
public:
    AddHpBuff();
    virtual void calc(FightUnit *);

public:
    int add_amount;
};

#endif