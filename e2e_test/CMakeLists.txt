
if (DEFINED BESM666__E2E_TESTS_BUILD)
    add_library(besm666-e2eif STATIC)
    target_sources(besm666-e2eif PRIVATE
        ./bootstrap.s
    )
    set_property(SOURCE ./bootstrap.s APPEND PROPERTY COMPILE_OPTIONS
        "-x" "assembler-with-cpp")
    set_property(TARGET besm666-e2eif PROPERTY LINK_DEPENDS 
        ${CMAKE_CURRENT_SOURCE_DIR}/e2e-linker.ld)
    target_link_options(besm666-e2eif PUBLIC
        "-T" "${CMAKE_CURRENT_SOURCE_DIR}/e2e-linker.ld"
        "-mcmodel=medany" "-fvisibility=hidden" "-nostdlib"
        "-nostartfiles" "-lgcc"
    )
    target_compile_options(besm666-e2eif PUBLIC
        "-march=rv64id" "-O2"
    )

    function(besm666_e2etest_asm SOURCE_NAME)
        get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
        add_executable(${TARGET_NAME})
        target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
        target_link_libraries(${TARGET_NAME} PRIVATE besm666-e2eif)
        set_property(SOURCE ${SOURCE_NAME} APPEND PROPERTY COMPILE_OPTIONS
            "-x" "assembler-with-cpp")
        add_test(
            NAME ${TARGET_NAME} 
            COMMAND ${CMAKE_BINARY_DIR}/../besm-666/standalone/besm666_standalone --a0-validation --executable ${TARGET_NAME}
        )
    endfunction(besm666_e2etest_asm)

    function(besm666_e2etest_c SOURCE_NAME)
        get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
        add_executable(${TARGET_NAME})
        target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
        target_include_directories(${TARGET_NAME} PRIVATE .)
        target_link_libraries(${TARGET_NAME} PRIVATE besm666-e2eif)
        add_test(
            NAME ${TARGET_NAME} 
            COMMAND ${CMAKE_BINARY_DIR}/../besm-666/standalone/besm666_standalone --a0-validation --executable ${TARGET_NAME}
        )
    endfunction(besm666_e2etest_c)

    besm666_e2etest_asm(./mult-test.s)
    besm666_e2etest_c(./bubblesort-test.c)
    besm666_e2etest_c(./primenumber-test.c)
endif()
