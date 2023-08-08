#ifndef __OBJECT_H__
#define __OBJECT_H__
#include <string>

class Object
{
public:
    virtual ~Object() {}
    virtual Object * clone() = 0;
    virtual Object * clone_and_clean() = 0;

public:
    std::string tag;    // 用于外部索引
    short idx;          // 用于对象池索引
};

#endif
