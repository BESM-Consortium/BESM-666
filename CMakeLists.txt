cmake_minimum_required(VERSION 3.22.1)

project(BESM-666 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



add_executable(besm-666)
target_sources(besm-666 PRIVATE
    ./src/main.cpp
)

add_subdirectory(third_party)

enable_testing()
add_subdirectory(t)
