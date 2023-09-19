add_library(besm666_utl STATIC)
target_sources(besm666_utl PRIVATE
    ./elf-parser.cpp
)
target_link_libraries(besm666_utl PRIVATE
    besm666_include
)
