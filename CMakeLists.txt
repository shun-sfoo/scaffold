cmake_minimum_required(VERSION 3.14)

project(scaffold LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFFJ)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(leptlog)
add_subdirectory(bench)
add_subdirectory(tests)
