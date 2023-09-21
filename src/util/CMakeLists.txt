
add_library(besm666_util STATIC)
target_sources(besm666_util PRIVATE
    ./elf-parser.cpp
)
target_include_directories(besm666_util PUBLIC
    # @todo #24:15m/DEV to add elfio as to target_link_libraries
    ../../third_party/ELFIO
)
target_link_libraries(besm666_util
PUBLIC
    elfio::elfio
PRIVATE
    besm666_include
)
