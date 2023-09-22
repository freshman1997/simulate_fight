#ifndef __ACTOR_H__
#define __ACTOR_H__
#include "object.h"
#include <vector>

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
    virtual void update(float deltaTime) = 0;
    virtual ~Actor() {}
    
    bool operator==(const Actor &actor) 
    {
        return actor.id == this->id;
    }

    bool operator!=(const Actor &actor) 
    {
        return actor.id != this->id;
    }

    // 一些初始化
    virtual bool init() { return true; }
    // 释放的时候调用
    virtual void deinit() {}
};

enum class ValueType
{
    int_val,
    float_val,
    double_val,
    bool_val,
};

#endif