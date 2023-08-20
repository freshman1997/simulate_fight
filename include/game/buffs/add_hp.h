#ifndef __ADD_HP_H__
#define __ADD_HP_H__

#include "../buff.h"

class AddHpBuff : public BuffBase
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();

public:
    AddHpBuff();
    virtual void calc(FightUnit *);

public:
    int add_amount;
};

#endif