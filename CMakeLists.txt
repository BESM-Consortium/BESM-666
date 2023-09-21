cmake_minimum_required(VERSION 3.22.1)

project(BESM-666 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python3 REQUIRED)

file(GLOB BESM666_RISCV_ISA_FILES 
    ${CMAKE_SOURCE_DIR}/third_party/riscv-opcodes/rv64_*)
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
    COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
    COMMENT "Creating include/besm-666/autogen directory"
)
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/include/besm-666/autogen/encoding.out.h
    COMMAND python3 ${CMAKE_SOURCE_DIR}/third_party/riscv-opcodes/parse.py -c 
        ${BESM666_RISCV_ISA_FILES}
    DEPENDS ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
    DEPENDS ${CMAKE_SOURCE_DIR}/third_party/riscv-opcodes/parse.py 
    DEPENDS ${BESM666_RISCV_ISA_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
    COMMENT "Generating RICSV instruction encoding info."
)
add_custom_target(besm666_riscv_isa DEPENDS 
    ${CMAKE_SOURCE_DIR}/include/besm-666/autogen/encoding.out.h)

add_library(besm666_include INTERFACE)
target_include_directories(besm666_include INTERFACE
    ./include
    # @todo #24:15m/DEV to add elfio as to target_link_libraries
    ./third_party/ELFIO
)
add_dependencies(besm666_include besm666_riscv_isa)

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
    besm666_util
)


add_executable(besm666)
target_sources(besm666 PRIVATE
    ./standalone/main.cpp
)
target_link_libraries(besm666 PRIVATE besm666::besm666_shared)

add_subdirectory(./src/memory)
add_subdirectory(src/util)
add_subdirectory(third_party)

enable_testing()
include (CTest)
if (BESM666_ENABLE_VALGRIND)
    set(CMAKE_CTEST_ARGUMENTS --test-action memcheck)
endif ()
add_subdirectory(test)
