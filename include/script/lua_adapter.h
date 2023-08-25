#ifndef __SCRIPT_LUA_H__
#define __SCRIPT_LUA_H__
#include <cstdlib>

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

class LuaInterpreter
{
public:
    LuaInterpreter();
    void excute_file(const char *name);
    ~LuaInterpreter()
    {
        if (!instance) {
            free(instance);
        }
    }
private:
    lua_State *instance = nullptr;
};

#endif
