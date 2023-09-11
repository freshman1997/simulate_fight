#ifndef __EQUIP_H__
#define __EQUIP_H__
#include "actor.h"

class HeroBase;

class EquipBase : public Actor
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();
    virtual void update(float deltaTime);

    // 进入战斗前计算
    virtual void on_begin();    

    // 受到伤害时
    virtual void on_being_hurt(int damage, int type);
    
public:
    HeroBase *owner;
    
    Vector2 pos;
    int extra_param1;
};

#endif
