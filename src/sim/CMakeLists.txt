

add_library(besm666_sim STATIC)
target_sources(besm666_sim PRIVATE
    ./config.cpp
    ./hart.cpp
    ./machine.cpp
    ./hooks.cpp
    ./plugin.cpp
)
target_link_libraries(besm666_sim PRIVATE
    besm666_include
    besm666_memory
    besm666_decoder
    besm666_exec
)
