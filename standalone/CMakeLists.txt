
add_executable(besm666_standalone)
add_executable(besm666::besm666_standalone ALIAS besm666_standalone)
target_sources(besm666_standalone PRIVATE
    ./main.cpp
)
target_link_libraries(besm666_standalone PRIVATE
    besm666_shared
    CLI11::CLI11
    capstone::capstone
)
