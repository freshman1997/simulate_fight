cmake_minimum_required (VERSION 3.5)
project(lua5.4)

AUX_SOURCE_DIRECTORY(./src/ ALL_SRC)
INCLUDE_DIRECTORIES(./src/)
message(${ALL_SRC})

SET(LIB_SRC ${ALL_SRC})
LIST(REMOVE_ITEM LIB_SRC lua.c luac.c)

ADD_LIBRARY(lua ${LIB_SRC})

ADD_EXECUTABLE(luaexec ./src/lua.c)
target_link_libraries(luaexec lua)
set_target_properties(luaexec PROPERTIES OUTPUT_NAME lua)

add_executable(luac ./src/luac.c)

target_link_libraries(luac lua)
