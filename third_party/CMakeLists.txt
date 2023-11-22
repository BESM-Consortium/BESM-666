set(CAPSTONE_RISCV_SUPPORT ON)

set(CAPSTONE_ARCHITECTURE_DEFAULT OFF)
set(CAPSTONE_RISCV_SUPPORT ON)

set(DIRS ELFIO googletest CLI11 capstone)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
