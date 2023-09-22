
add_library(besm666_util STATIC)
target_sources(besm666_util PRIVATE
    ./elf-parser.cpp
)
target_link_libraries(besm666_util
PUBLIC
    elfio::elfio
PRIVATE
    besm666_include
)
