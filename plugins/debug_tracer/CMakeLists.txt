project(BESM-666-DebugTracer
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Plugin used to debug the simulator"
)

add_library(besm666_debug_tracer SHARED)
target_sources(besm666_debug_tracer PRIVATE
    ./debug-tracer.cpp
)
target_link_libraries(besm666_debug_tracer PRIVATE
    besm666_include
    capstone::capstone
    CLI11::CLI11
)
