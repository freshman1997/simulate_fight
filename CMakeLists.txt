cmake_minimum_required (VERSION 3.5)
project(simulate_fight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include_directories(include/ 3rdparty/gtest/googletest/include 3rdparty/json/include 3rdparty/lua-5.4.6/src)

FILE(GLOB_RECURSE GAME_SRC "./src/*.cpp")

enable_testing()

add_subdirectory(3rdparty/gtest)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/lua-5.4.6)

add_executable(test_add tests/test1.cpp)
target_link_libraries(test_add gtest gtest_main)

add_executable(test_json tests/test_json.cpp)
target_link_libraries(test_json gtest gtest_main)

add_test(NAME test_add COMMAND test_add)
add_test(NAME test_json COMMAND test_json)

add_executable(fight ${GAME_SRC}) 
