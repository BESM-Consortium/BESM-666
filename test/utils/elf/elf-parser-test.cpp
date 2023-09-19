#include <gtest/gtest.h>

#include "besm-666/utils/elf-parser.hpp"

void generateValidElf() {
    ELFIO::elfio writer;

    writer.create( ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB );

    writer.set_os_abi( ELFIO::ELFOSABI_LINUX );
    writer.set_type( ELFIO::ET_EXEC );
    writer.set_machine( ELFIO::EM_386 );

    ELFIO::section* text_sec = writer.sections.add( ".text" );
    text_sec->set_type( ELFIO::SHT_PROGBITS );
    text_sec->set_flags( ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR );
    text_sec->set_addr_align( 0x10 );

    char text[] = { '\xB8', '\x04', '\x00', '\x00', '\x00',
                   '\xBB', '\x01', '\x00', '\x00', '\x00',
                   '\xB9', '\x20', '\x80', '\x04', '\x08',
                   '\xBA', '\x0E', '\x00', '\x00', '\x00',
                   '\xCD', '\x80',
                   '\xB8', '\x01', '\x00', '\x00', '\x00',
                   '\xCD', '\x80' };
    text_sec->set_data( text, sizeof( text ) );

    ELFIO::segment* text_seg = writer.segments.add();
    text_seg->set_type( ELFIO::PT_LOAD );
    text_seg->set_virtual_address( 0x08048000 );
    text_seg->set_physical_address( 0x08048000 );
    text_seg->set_flags( ELFIO::PF_X | ELFIO::PF_R );
    text_seg->set_align( 0x1000 );

    text_seg->add_section_index( text_sec->get_index(),
                                text_sec->get_addr_align() );

    ELFIO::section* data_sec = writer.sections.add( ".data" );
    data_sec->set_type( ELFIO::SHT_PROGBITS );
    data_sec->set_flags( ELFIO::SHF_ALLOC | ELFIO::SHF_WRITE );
    data_sec->set_addr_align( 0x4 );
    char data[] = { '\x48', '\x65', '\x6C', '\x6C', '\x6F',
                   '\x2C', '\x20', '\x57', '\x6F', '\x72',
                   '\x6C', '\x64', '\x21', '\x0A' };
    data_sec->set_data( data, sizeof( data ) );
    ELFIO::segment* data_seg = writer.segments.add();
    data_seg->set_type( ELFIO::PT_LOAD );
    data_seg->set_virtual_address( 0x08048020 );
    data_seg->set_physical_address( 0x08048020 );
    data_seg->set_flags( ELFIO::PF_W | ELFIO::PF_R );
    data_seg->set_align( 0x10 );

    data_seg->add_section_index( data_sec->get_index(),
                                data_sec->get_addr_align() );
    writer.set_entry( 0x08048000 );
    writer.save( "hello_i386_32" );
}

void generateInvalidElf() {
    ELFIO::elfio writer;

    writer.create( ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB );

    writer.set_os_abi( ELFIO::ELFOSABI_LINUX );
    writer.set_type( ELFIO::ET_EXEC );
    writer.set_machine( ELFIO::EM_386 );

    ELFIO::section* text_sec = writer.sections.add( ".text" );
    text_sec->set_type( ELFIO::SHT_PROGBITS );
    text_sec->set_flags( ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR );
    text_sec->set_addr_align( 0x10 );

    char text[] = { '\xB8', '\x04', '\x00', '\x00', '\x00',
                   '\xBB', '\x01', '\x00', '\x00', '\x00',
                   '\xB9', '\x20', '\x80', '\x04', '\x08',
                   '\xBA', '\x0E', '\x00', '\x00', '\x00',
                   '\xCD', '\x80',
                   '\xB8', '\x01', '\x00', '\x00', '\x00',
                   '\xCD', '\x80' };
    text_sec->set_data( text, sizeof( text ) );

    ELFIO::segment* text_seg = writer.segments.add();
    text_seg->set_type( ELFIO::PT_LOAD );
    text_seg->set_virtual_address( 0x08048000 );
    text_seg->set_physical_address( 0x08048000 );
    text_seg->set_flags( ELFIO::PF_X | ELFIO::PF_R );
    text_seg->set_align( 0x1000 );

    text_seg->add_section_index( text_sec->get_index(),
                                text_sec->get_addr_align() );

    ELFIO::section* data_sec = writer.sections.add( ".data" );
    data_sec->set_type( ELFIO::SHT_PROGBITS );
    data_sec->set_flags( ELFIO::SHF_ALLOC | ELFIO::SHF_WRITE );
    data_sec->set_addr_align( 0x4 );
    char data[] = { '\x48', '\x65', '\x6C', '\x6C', '\x6F',
                   '\x2C', '\x20', '\x57', '\x6F', '\x72',
                   '\x6C', '\x64', '\x21', '\x0A' };
    data_sec->set_data( data, sizeof( data ) );
    ELFIO::segment* data_seg = writer.segments.add();
    data_seg->set_type( ELFIO::PT_LOAD );
    data_seg->set_virtual_address( 0x08048020 );
    data_seg->set_physical_address( 0x08048020 );
    data_seg->set_flags( ELFIO::PF_W | ELFIO::PF_R );
    data_seg->set_align( 0x10 );

    data_seg->add_section_index( data_sec->get_index(),
                                data_sec->get_addr_align() );
    writer.set_entry( 0x08048000 );
    writer.save( "hello_i386_32" );
}

TEST(elf_parser_test, requirements_test) {
    EXPECT_TRUE(true);
}