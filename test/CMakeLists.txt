
function(besm666_simple_test SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE gtest_main gtest)
    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction(besm666_simple_test)

besm666_simple_test(./dummy_test.cpp)
