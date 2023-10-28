
add_library(besm666_decoder STATIC)
target_sources(besm666_decoder PRIVATE
    decoder.cpp
    prefetcher.cpp
)
target_link_libraries(besm666_decoder PRIVATE
    besm666_include
    besm666_memory
)
