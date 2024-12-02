cmake_minimum_required(VERSION 3.14)

project(scaffold LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFFJ)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        11.0.2
)

FetchContent_MakeAvailable(fmt)

add_subdirectory(leptlog)
add_subdirectory(tests)
add_subdirectory(learning)
