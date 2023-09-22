cmake_minimum_required (VERSION 3.5)
project(simulate_fight)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#add_compile_options()

include_directories(
    include/ 
    game_proto/
    3rdparty/gtest/googletest/include 
    3rdparty/json/include 
    3rdparty/lua-5.4.6/src
    3rdparty/protobuf/third_party/abseil-cpp/
    3rdparty/protobuf/src/
)

#"./game_proto/proto/*.cc"
FILE(GLOB_RECURSE GAME_SRC "./src/*.cpp")
LIST(REMOVE_ITEM GAME_SRC ./src/script/lua_lib.cpp)

enable_testing()

add_subdirectory(3rdparty/gtest)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/lua-5.4.6)
add_subdirectory(3rdparty/protobuf)

if(UNIX)
    include_directories(3rdparty/libev)
    add_subdirectory(3rdparty/libev)
endif()

add_executable(test_add tests/test1.cpp)
target_link_libraries(test_add gtest gtest_main)

add_executable(test_json tests/test_json.cpp)
target_link_libraries(test_json gtest gtest_main)

add_test(NAME test_add COMMAND test_add)
add_test(NAME test_json COMMAND test_json)

add_library(game SHARED ./src/script/lua_lib.cpp)
target_link_libraries(game PUBLIC lua5.4)

add_executable(fight ${GAME_SRC} main.cpp) 
target_link_libraries(fight PUBLIC lua5.4)
#target_link_directories(fight PUBLIC build/3rdparty/protobuf)
#target_link_libraries(fight PUBLIC protobufd)
#add_executable(iocp_test iocp_test.cpp)