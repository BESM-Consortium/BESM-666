
add_library(besm666_testif INTERFACE)
target_link_libraries(besm666_testif INTERFACE gmock_main gmock gtest)

function(besm666_test SOURCE_NAME)
    function(besm666_simple_test SOURCE_NAME)
        get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
        add_executable(${TARGET_NAME})
        target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
        target_link_libraries(${TARGET_NAME} PRIVATE besm666_testif)
        add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
    endfunction(besm666_simple_test)

    function(besm666_valgrind_test SOURCE_NAME)
        get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
        add_executable(${TARGET_NAME}_valgrind)
        target_sources(${TARGET_NAME}_valgrind PRIVATE ${SOURCE_NAME})
        target_link_libraries(${TARGET_NAME}_valgrind PRIVATE besm666_testif)
        add_test(NAME ${TARGET_NAME}_valgrind COMMAND valgrind ./${TARGET_NAME}_valgrind)
    endfunction(besm666_valgrind_test)

    besm666_simple_test(${SOURCE_NAME})

    if (BESM666_ENABLE_VALGRIND)
        besm666_valgrind_test(${SOURCE_NAME})
    endif ()
endfunction(besm666_test)

besm666_test(./dummy_test.cpp)

