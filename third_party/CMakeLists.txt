set(DIRS ELFIO googletest)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
