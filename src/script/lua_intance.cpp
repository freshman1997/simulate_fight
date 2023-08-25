#include <iostream>
#include "script/lua_adapter.h"
using namespace std;

LuaInterpreter::LuaInterpreter()
{
    instance = luaL_newstate();
    luaL_openlibs(instance);
}


void LuaInterpreter::excute_file(const char *name)
{
    if (LUA_OK != luaL_dofile(instance, name)) {
        cout << "failed!!\n";
    } else {
        cout << "success!!\n";
    }
}

