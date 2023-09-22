add_executable(besm666_elf_parser_tests)
target_sources(besm666_elf_parser_tests PRIVATE
    ./elf-parser-test.cpp
)
target_link_libraries(besm666_elf_parser_tests PRIVATE
    besm666_testif
    besm666_util
    besm666_elfgen
)
add_test(NAME besm666_elf_parser COMMAND besm666_elf_parser_tests)
