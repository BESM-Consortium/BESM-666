

add_library(besm666_exec STATIC)
target_sources(besm666_exec PRIVATE
    ./gprf.cpp
    ./csrf.cpp
    ./icsr.cpp
    ./basic-block.cpp
)
target_link_libraries(besm666_exec PRIVATE 
    besm666_include
)
