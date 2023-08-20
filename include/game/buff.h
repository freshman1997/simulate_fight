#ifndef __BUFF_H__
#define __BUFF_H__
#include "actor.h"

class FightUnit;
struct Buff;

enum class buff_type : char
{
    none = -1,
    hp,
};

enum class buff_sub_type : char
{
    none = 0,
};

enum class buff_func_type : char
{
    none = 0,
};

enum class buff_trigger_condition : char
{
    none = 0,
};

enum class buff_target_choose_rule : char
{
    none = 0,
};

enum class buff_target_sort_type : char
{
    none = 0,
};

enum class add_type : char
{
    
};

class BuffBase : public Actor
{
public:
    virtual void update(float deltaTime);
    virtual Object * clone();
    virtual Object * clone_and_clean();
    virtual void calc(FightUnit *);

    virtual void on_add() { buff_time = 0; }
    virtual void on_remove() {}
    void reset();
    void free();

public:
    const Buff *buff_cfg = nullptr;
    FightUnit *from = nullptr;
    FightUnit *to = nullptr;
    float buff_time = 0;
};

#endif
