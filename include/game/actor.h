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

public:
    int id = 0;
};

enum class ValueType
{
    int_val,
    float_val,
    double_val,
    bool_val,
};

union Val
{
    bool bval;
    int ival;
    float fval;
    double dval;
};

struct Value
{
    ValueType type = ValueType::int_val;
    Val base_val;
    Val change_val;

    template<class T>
    T get_value()
    {
        switch (type)
        {
        case ValueType::int_val:
            return T(base_val.ival + change_val.ival);
        case ValueType::float_val:
            return T(base_val.fval + change_val.fval);
        case ValueType::double_val:
            return T(base_val.dval + change_val.dval);
        case ValueType::bool_val:
            return T(change_val.bval);
        
        default:
            break;
        }

        return T(base_val.ival + change_val.ival);
    }
};

class ValueHandler
{
public:
    virtual ~ValueHandler() {}
    virtual bool remove() = 0;
    virtual void on_value_changed(Value &) = 0;
};

#endif