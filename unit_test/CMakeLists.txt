
add_library(besm666_testif INTERFACE)
target_link_libraries(besm666_testif INTERFACE 
    besm666_include
    besm666_exec 
    besm666_memory 
    besm666_decoder
    besm666_util
    besm666_sim
    gmock_main 
    gmock 
    gtest
)

function(besm666_test SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE besm666_testif)
    add_test(NAME ${TARGET_NAME} COMMAND ./${TARGET_NAME})
endfunction(besm666_test)

besm666_test(./dummy_test.cpp)

set(DIRS decoder exec memory util elfgen)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()
