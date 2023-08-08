#ifndef __BUFF_H__
#define __BUFF_H__
#include "actor.h"

class FightUnit;

enum class buff_type : unsigned char
{
    none = 0,
    hp,
};

enum class add_type
{
    
};

class BaseBuff : public Actor
{
public:
    BaseBuff() : BaseBuff(buff_type::none){}
    BaseBuff(buff_type t) : type(t), infinite(false) {}

    virtual Object * clone() = 0;
    virtual Object * clone_and_clean() = 0;
    virtual void calc(FightUnit *) = 0;

    virtual void on_add() {}
    virtual void on_remove() {}

public:
    bool infinite;
    buff_type type;
    FightUnit *from;
    FightUnit *to;
    float param1;
    float param2;
    float param3;
};

#endif
