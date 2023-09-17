add_library(besm666_elf STATIC)
target_sources(besm666_elf PRIVATE
    ./elf-parser.cpp
)
target_link_libraries(besm666_elf PRIVATE
    besm666_include
)
