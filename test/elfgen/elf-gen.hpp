#pragma once

#include "besm-666/utils/elf-parser.hpp"

namespace besm::gen {
using elf_t = unsigned char;

constexpr char defaultData[] = {'\x62', '\x6F', '\x6F', '\x62', '\x73', '\x00'};
constexpr RV64Ptr defaultPtr = 0xDEADEBABE;

void generateElf(const std::filesystem::path &path, elf_t fileClass,
                 elf_t encoding, elf_t arch, const char loadData[],
                 size_t loadSize, RV64Ptr loadPtr) {
    ELFIO::elfio writer;

    writer.create(fileClass, encoding);
    writer.set_type(ELFIO::ET_EXEC);
    writer.set_os_abi(ELFIO::ELFOSABI_LINUX);
    writer.set_machine(arch);

    ELFIO::section *text_sec = writer.sections.add(".text");
    text_sec->set_type(ELFIO::SHT_PROGBITS);
    text_sec->set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR);

    text_sec->set_data(loadData, loadSize);

    ELFIO::segment *text_seg = writer.segments.add();
    text_seg->set_type(ELFIO::PT_LOAD);
    text_seg->set_virtual_address(loadPtr);
    text_seg->set_physical_address(loadPtr);
    text_seg->set_flags(ELFIO::PF_X | ELFIO::PF_R);

    text_seg->add_section_index(text_sec->get_index(),
                                text_sec->get_addr_align());

    writer.set_entry(loadPtr);
    writer.save(path);
}

void generateSuitableElf(const std::filesystem::path &path,
                         const char *loadData = defaultData,
                         size_t loadSize = sizeof(defaultData),
                         RV64Ptr loadPtr = defaultPtr) {
    using namespace besm::utl;
    generateElf(path, ElfParser::Requirements::FileClass,
                ElfParser::Requirements::Encoding,
                ElfParser::Requirements::Arch, loadData, loadSize, loadPtr);
}

void generateUnsuitableElf(const std::filesystem::path &path, elf_t fileClass,
                           elf_t encoding, elf_t arch,
                           const char *loadData = defaultData,
                           size_t loadSize = sizeof(defaultData),
                           RV64Ptr loadPtr = defaultPtr) {
    using namespace besm::utl;
    generateElf(path, fileClass, encoding, arch, loadData, loadSize, loadPtr);
}
}; // namespace besm::gen
