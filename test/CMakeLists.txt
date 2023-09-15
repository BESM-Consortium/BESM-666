
add_library(besm666_testif INTERFACE)
target_link_libraries(besm666_testif INTERFACE besm666_shared gmock_main gmock gtest)

function(besm666_test SOURCE_NAME)
    function(init_test SOURCE_NAME COMMAND POSTFIX)
        get_filename_component(FILENAME ${SOURCE_NAME} NAME_WE)
        set(TARGET_NAME ${FILENAME}_${POSTFIX})
        add_executable(${TARGET_NAME})
        target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
        target_link_libraries(${TARGET_NAME} PRIVATE besm666_testif)
        add_test(NAME ${TARGET_NAME} COMMAND ${COMMAND} ./${TARGET_NAME})
    endfunction(init_test)

    init_test(${SOURCE_NAME} "" "")

    if (BESM666_ENABLE_VALGRIND)
        init_test(${SOURCE_NAME} valgrind valgrind)
    endif ()
endfunction(besm666_test)

besm666_test(./dummy_test.cpp)
besm666_test(./reg/gprf-tests.cpp)
