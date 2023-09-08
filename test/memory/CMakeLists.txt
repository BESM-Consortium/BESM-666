
add_executable(besm666_page_pool_tests)
target_sources(besm666_page_pool_tests PRIVATE ./page-pool-tests.cpp)
target_link_libraries(besm666_page_pool_tests PRIVATE 
    besm666_testif
    besm666_memory
)
target_link_options(besm666_page_pool_tests PRIVATE
    -Wl,--wrap=besm666_mmap
    -Wl,--wrap=besm666_munmap
)

add_test(NAME besm666_page_pool COMMAND besm666_page_pool_tests)
