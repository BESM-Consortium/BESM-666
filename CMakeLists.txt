cmake_minimum_required(VERSION 3.22.1)

project(BESM-666 
    LANGUAGES ASM CXX
    VERSION 1.0.0
    DESCRIPTION "Pet RISCV Simulator"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(DEFINED BESM666_WITH_E2E_TESTS AND NOT DEFINED BESM666_RISCV_SYSROOT)
    message(FATAL_ERROR "BESM666_RISCV_SYSROOT should be defined")
endif()

if(NOT DEFINED BESM666__PARENT_BUILD)
    include(ExternalProject)

    ExternalProject_Add(besm666_simulator
        SOURCE_DIR ${CMAKE_SOURCE_DIR}
        BINARY_DIR ${CMAKE_BINARY_DIR}/besm-666
        CMAKE_CACHE_ARGS
            -DBESM666__SIMULATOR_BUILD:BOOL=ON
            -DBESM666__PARENT_BUILD:BOOL=ON
        INSTALL_COMMAND ""
        BUILD_ALWAYS ON
    )

    if(DEFINED BESM666_WITH_E2E_TESTS)
        ExternalProject_Add(besm666_e2e
            SOURCE_DIR ${CMAKE_SOURCE_DIR}
            BINARY_DIR ${CMAKE_BINARY_DIR}/e2e
            CMAKE_CACHE_ARGS
                -DBESM666__PARENT_BUILD:BOOL=ON
                -DBESM666__E2E_TESTS_BUILD:BOOL=ON
                -DBESM666_RISCV_SYSROOT:FILEPATH=${BESM666_RISCV_SYSROOT}
                -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_SOURCE_DIR}/tools/riscv64-toolchain.make
            INSTALL_COMMAND ""
            BUILD_ALWAYS ON
        )
    endif()

    return()
endif()

if(DEFINED BESM666__SIMULATOR_BUILD)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    find_package(Python3 REQUIRED)
    find_package(Git REQUIRED)

    message(STATUS "Fetching submodules")
    #execute_process(
    #COMMAND ${Git_EXECUTABLE} submodule update --init --recursive
    #WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    #RESULT_VARIABLE GIT_SUBMODULE_UPDATE_RESULT
    #)
    #if(NOT GIT_SUBMODULE_UPDATE_RESULT EQUAL "0")
    #message(FATAL_ERROR "Failed to fetch submodules")
    #endif()

    file(GLOB BESM666_RISCV_ISA_FILES 
        ${CMAKE_SOURCE_DIR}/third_party/riscv-opcodes/rv64_*)
    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
        COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
        COMMENT "Creating include/besm-666/autogen directory"
    )
    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/include/besm-666/autogen/encoding.out.h
        COMMAND ${Python3_EXECUTABLE} 
            ${CMAKE_SOURCE_DIR}/third_party/riscv-opcodes/parse.py -c 
            ${BESM666_RISCV_ISA_FILES}
        DEPENDS ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
        DEPENDS ${CMAKE_SOURCE_DIR}/third_party/riscv-opcodes/parse.py 
        DEPENDS ${BESM666_RISCV_ISA_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include/besm-666/autogen
        COMMENT "Generating RICSV instruction encoding info."
    )
    add_custom_target(besm666_riscv_isa DEPENDS 
        ${CMAKE_SOURCE_DIR}/include/besm-666/autogen/encoding.out.h)

    add_subdirectory(src)
    add_subdirectory(standalone)
    add_subdirectory(third_party)

    enable_testing()
    if (DEFINED BESM666_TEST_WITH_VALGRIND)
        include (CTest)
        set(CMAKE_CTEST_ARGUMENTS --test-action memcheck)
    endif()

    add_subdirectory(unit_test)
endif()

if (DEFINED BESM666__E2E_TESTS_BUILD)
    enable_testing()
endif()
add_subdirectory(e2e_test)
