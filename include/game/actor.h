#ifndef __ACTOR_H__
#define __ACTOR_H__

struct Vector2
{
    Vector2() : x(0), y(0){}
    float x;
    float y;

    friend Vector2 & operator + (Vector2 &);
    friend Vector2 & operator - (Vector2 &);
    friend Vector2 & operator * (Vector2 &);
    friend Vector2 & operator / (Vector2 &, Vector2 &);
    friend bool operator == (Vector2 &, Vector2 &);
    friend bool operator != (Vector2 &, Vector2 &);
};

class Actor
{
public:
    virtual void update() {}
    virtual ~Actor() {}
public:
    int id = 0;
};

#endif