extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#include <thread>
#include <chrono>
#include <iostream>

static int game_sleep (lua_State *L) {
  if (lua_isnumber(L, -1)) {
    int ts = lua_tointegerx(L, -1, nullptr);
    if (ts > 0) {
        std::chrono::duration<double, std::milli> mills(ts);
        std::this_thread::sleep_for(mills);
        return 1;
    }
  }

  return 0;
}


static const luaL_Reg lualib[] = {
  {"sleep",     game_sleep},
  {NULL, NULL}
};

extern "C" {
    __declspec(dllexport) int luaopen_libgame (lua_State *L) {
        luaL_newlib(L, lualib);
        return 1;
    }
}