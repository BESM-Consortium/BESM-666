
add_library(besm666_memory STATIC)
target_sources(besm666_memory PRIVATE
    ./page-allocator.cpp
    ./phys-mem.cpp
    ./mmap-wrapper.cpp
    ./mmu.cpp
)
target_link_libraries(besm666_memory PRIVATE
    besm666_include
    besm666_util
)
