#ifndef __SCRIPT_LUA_H__
#define __SCRIPT_LUA_H__
#include <crtdefs.h>
#include <functional>
#include <map>
#include <unordered_map>
#include <set>
#include <cstdlib>

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

class LuaAdapter
{
public:
    LuaAdapter();
    bool excute_file(const char *name);
    ~LuaAdapter()
    {
        if (!instance) {
            free(instance);
        }
    }

    bool init();
    static int lua_error_handler(lua_State* l);

    void pushValue(lua_State* L, int value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, unsigned int value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, short value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, unsigned short value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, signed char value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, unsigned char value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, float value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, double value) { lua_pushnumber(L, value); }
    void pushValue(lua_State* L, uint64_t value) { lua_pushinteger(L, value); }
    void pushValue(lua_State* L, int64_t value) { lua_pushinteger(L, value); }
    void pushValue(lua_State* L, const char* value) { lua_pushstring(L, value); }
    void pushValue(lua_State* L, const std::string& value) { lua_pushlstring(L, value.c_str(), value.size()); }
    void pushValue(lua_State* L, bool value) { lua_pushboolean(L, value ? 1 : 0); }
    void pushValue(lua_State* L, lua_CFunction f) { lua_pushcfunction(L, f); }

    template<typename T>
    void pushValue(lua_State* L, const std::vector<T>& value) {
        lua_newtable(L);
        auto size = value.size();
        for (std::size_t i = 0; i < size; ++i) {
            pushValue(L, value[i]);
            lua_rawseti(L, -2, (int)i + 1);
        }
    }

    template<typename T> 
    void pushValue(lua_State* L, const std::set<T>& value) {
        lua_newtable(L);
        for (auto iter = value.begin(); iter != value.end(); ++iter) {
            pushValue(L, *iter);
            pushValue(L, true);
            lua_rawset(L, -3);
        }
    }

    template<typename TKey, typename TValue> 
    void pushValue(lua_State* L, const std::map<TKey, TValue>& value) {
        lua_newtable(L);
        for (auto iter = value.begin(); iter != value.end(); ++iter) {
            pushValue(L, iter->first);
            pushValue(L, iter->second);
            lua_rawset(L, -3);
        }
    }

    unsigned int pushValues(lua_State* L) {
        return 0;
    }

    template<typename Arg, typename... Args> 
    unsigned int pushValues(lua_State* L, Arg arg, Args ... args) {
        pushValue(L, arg);
        return pushValues(L, args ...) + 1;
    }

    template<typename Result>
    Result get_result(bool &result) {
        if (!lua_isnumber(this->instance, -1)) {
            return (Result)0;
        }

        result = true;
        return static_cast<Result>(lua_tonumber(this->instance, -1));
    }

    template<class Result, typename ...Args>
    Result call_lua(const char *tb_name, const char *func, Args ...args)
    {
        lua_pushcfunction(this->instance, &LuaAdapter::lua_error_handler);
        int err_fun_index = lua_gettop(this->instance);

        if (try_get_table(this->instance, tb_name, func)) {
            return Result();
        }

        if (!lua_isfunction(this->instance, -1)) {
            return Result();
        }

        bool result = false;
        unsigned int arg_cnt = pushValues(this->instance, args...);

        int ret = lua_pcall(this->instance, arg_cnt, 1, err_fun_index);

        if (ret == 0) {
            return get_result<Result>(this->instance, result);
        } else {
            result = false;
            return Result();
        }
    }

    bool try_get_table(lua_State* L, const char* tableName, const char* valueName);

    // TODO
    void reload(const char *name);

private:
    const char *cwd;
    const char *reload_file;
    lua_State *instance;
};

///////////////////////////////////////////

template<>
inline void LuaAdapter::get_result<void> (bool &result) {
    result = true;
    return;
}

template<>
inline const char * LuaAdapter::get_result<const char*>(bool &result) {
    if (!lua_isstring(this->instance, -1)) {
        return "";
    }

    result = true;
    return lua_tostring(this->instance, -1);
}

template<>
inline std::string LuaAdapter::get_result<std::string>(bool &result) {
    if (!lua_isstring(this->instance, -1)) {
        return "";
    }

    result = true;
    std::string res;

    size_t len = 0;
    const char *tstr = lua_tolstring(this->instance, -1, &len);
    res.assign(tstr);

    return res;
}

template<>
inline bool LuaAdapter::get_result<bool>(bool &result) {
    if (!lua_isboolean(this->instance, -1)) {
        return "";
    }

    result = true;
    return lua_toboolean(this->instance, -1) ? true : false;
}

#endif
