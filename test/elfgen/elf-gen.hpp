#pragma once

#include "besm-666/util/elf-parser.hpp"

namespace besm::gen {
using elf_t = unsigned char;

constexpr char defaultData[] = {'\x62', '\x6F', '\x6F', '\x62', '\x73', '\x00'};
constexpr RV64Ptr defaultPtr = 0xDEADEBABE;

enum Requirements {
    /// 64bit
    FileClass = ELFIO::ELFCLASS64,
    /// little-endian
    Encoding = ELFIO::ELFDATA2LSB,
    /// RISC-V
    Arch = ELFIO::EM_RISCV
};

/**
 * \brief generates ELF file with given parameters and puts it in a file
 *
 * \param [in] path path to result file
 * \param [in] fileClass class of result ELF file (e.g. 32bit or 64bit)
 * \param [in] encoding little endian or big endian
 * \param [in] arch target architecture of result ELF file
 * \param [in] loadData data to be loaded in LOAD segments of result ELF
 * \param [in] loadSize size of the `loadData`
 * \param [in] loadPtr virtual pointer of LOAD segment and entry point
 */
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

/**
 * \brief generates ELF file that suites all simulator requirements
 */
void generateSuitableElf(const std::filesystem::path &path,
                         const char *loadData = defaultData,
                         size_t loadSize = sizeof(defaultData),
                         RV64Ptr loadPtr = defaultPtr) {
    using namespace besm::util;
    generateElf(path, Requirements::FileClass, Requirements::Encoding,
                Requirements::Arch, loadData, loadSize, loadPtr);
}

/**
 * \brief generates ELF file that doesn't suite simulator requirements
 */
void generateUnsuitableElf(const std::filesystem::path &path, elf_t fileClass,
                           elf_t encoding, elf_t arch,
                           const char *loadData = defaultData,
                           size_t loadSize = sizeof(defaultData),
                           RV64Ptr loadPtr = defaultPtr) {
    using namespace besm::util;
    generateElf(path, fileClass, encoding, arch, loadData, loadSize, loadPtr);
}
}; // namespace besm::gen
