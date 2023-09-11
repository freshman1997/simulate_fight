#include <iostream>
#include "script/lua_adapter.h"

LuaAdapter::LuaAdapter()
{
    cwd = "../lua_source/";
    reload_file = "env/reload.lua";
    instance = luaL_newstate();
    init();
}

bool LuaAdapter::init()
{
    if (!instance) {
        return false;
    }

    std::string env_file = cwd + std::string("env/set_env.lua");
    if (excute_file(env_file.c_str())) {
        luaL_openlibs(instance);
        return true;
    } else {
        return false;
    }
}

int LuaAdapter::lua_error_handler(lua_State* l)
{

    return 0;
}

bool LuaAdapter::excute_file(const char *name)
{
    return LUA_OK != luaL_dofile(instance, name);
}

bool LuaAdapter::try_get_table(lua_State* L, const char* tableName, const char* valueName)
{
    if (tableName == nullptr) {
        lua_getglobal(L, valueName);
    }
    else {
        lua_getglobal(L, tableName);

        if (1 != lua_istable(L, -1)) {
            return false;
        }

        lua_pushstring(L, valueName);
        lua_rawget(L, -2);
    }

    return true;
}

void LuaAdapter::reload(const char *name)
{

}
