#ifndef __ACTOR_H__
#define __ACTOR_H__
#include "object.h"
struct Vector2
{
    Vector2() : x(0), y(0){}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    float x;
    float y;
};

class Actor : public Object
{
public:
    virtual void update(float deltaTime) {}
    virtual ~Actor() {}
    
public:
    int id = 0;
};

#endif