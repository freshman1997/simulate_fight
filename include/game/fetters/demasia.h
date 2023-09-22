#ifndef __DEMASIA_H__
#define __DEMASIA_H__

#include "../fetter.h"

class EquipmentBase;

class Demasia : public FetterBase
{
public:
    virtual Object * clone();
    virtual Object * clone_and_clean();

    virtual void update(float deltaTime);

    virtual bool init();    
    
    virtual void deinit();    

    virtual void on_begin();

    // 正式开启战斗之前
    virtual void on_before_round();
public:
    void generate_rand_equips();

};

#endif