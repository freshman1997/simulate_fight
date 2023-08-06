#ifndef __BUFFF_MANAGER_H__
#define __BUFFF_MANAGER_H__
#include "game/buff.h"
#include <unordered_map>

class BuffManager
{

private:
    std::unordered_map<int, BaseBuff *> buffs;
};

#endif