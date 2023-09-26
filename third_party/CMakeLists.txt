set(DIRS ELFIO googletest CLI11)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
