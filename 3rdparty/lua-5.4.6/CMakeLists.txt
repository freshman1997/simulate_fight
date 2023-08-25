cmake_minimum_required (VERSION 3.5)
project(lua5.4)

AUX_SOURCE_DIRECTORY(./src/ ALL_SRC)
INCLUDE_DIRECTORIES(./src/)

SET(LIB_SRC ${ALL_SRC})
LIST(REMOVE_ITEM LIB_SRC ./src//lua.c ./src//luac.c)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../build/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)

#ADD_LIBRARY(${PROJECT_NAME} ${LIB_SRC})
add_library(${PROJECT_NAME} SHARED ${LIB_SRC})

#ADD_EXECUTABLE(luaexec ./src/lua.c)
#target_link_libraries(luaexec lua)
#set_target_properties(luaexec PROPERTIES OUTPUT_NAME lua)

#add_executable(luac ./src/luac.c)

#target_link_libraries(luac lua)
