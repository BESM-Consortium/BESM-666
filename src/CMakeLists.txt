add_library(besm666_include INTERFACE)
target_include_directories(besm666_include INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_compile_options(besm666_include INTERFACE "-O2")
add_dependencies(besm666_include besm666_riscv_isa)

add_library(besm666_shared SHARED)
target_sources(besm666_shared PRIVATE
    stub.cpp
)

target_link_libraries(besm666_shared
PUBLIC
    besm666_include
    besm666_memory
    besm666_exec
    besm666_decoder
    besm666_sim
)
target_link_options(besm666_shared PRIVATE
    "-Wl,-export-dynamic"
)

add_subdirectory(memory)
add_subdirectory(util)
add_subdirectory(exec)
add_subdirectory(decoder)
add_subdirectory(sim)

