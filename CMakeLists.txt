cmake_minimum_required(VERSION 3.22.1)

project(BESM-666 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(besm666_include INTERFACE)
target_include_directories(besm666_include INTERFACE
    ./include
    ./third_party/ELFIO #TODO: to add elfio as to target_link_libraries
)

add_library(besm666_shared SHARED)
add_library(besm666::besm666_shared ALIAS besm666_shared)
target_sources(besm666_shared PRIVATE
    ./src/stub.cpp
)

target_link_libraries(besm666_shared
PUBLIC
    besm666_include
PRIVATE
    elfio::elfio
    besm666_memory
    besm666_utl
)

add_executable(besm666)
target_sources(besm666 PRIVATE
    ./standalone/main.cpp
)
target_link_libraries(besm666 PRIVATE besm666::besm666_shared)

add_subdirectory(./src/memory)
add_subdirectory(./src/utils)
add_subdirectory(third_party)

enable_testing()
include (CTest)
if (BESM666_ENABLE_VALGRIND)
    set(CMAKE_CTEST_ARGUMENTS --test-action memcheck)
endif ()
add_subdirectory(test)
