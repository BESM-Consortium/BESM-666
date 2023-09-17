#include "besm-666/utils/elf-parser.hpp"

namespace besm::elf {

ElfParser::ElfParser(const std::filesystem::path &elfPath) {
    if (!reader_.load(elfPath)) {
        throw BadElf("Invalid ELF file: " + elfPath.string());
    }
    checkRequirements();
}
void ElfParser::checkRequirements() {
    if (reader_.get_class() != Requirements::FileClass) {
        throw BadElf("Wrong ELF file class.");
    }
    if (reader_.get_encoding() != Requirements::Encoding) {
        throw BadElf("Wrong ELF encoding.");
    }
    if (reader_.get_machine() != Requirements::Arch) {
        throw BadElf("Wrong ELF target architecture.");
    }
}
/**
 * @todo #9:15m/DEV to check that elf does not require dynamic linking
 */
const std::vector<ElfParser::LoadableSegment> &
ElfParser::getLoadableSegments() {
    for (const auto &seg : reader_.segments) {
        if (seg->get_type() == ELFIO::PT_LOAD) {
            loadableSegments_.push_back(
                LoadableSegment{seg->get_virtual_address(), seg->get_data(),
                                static_cast<RV64Size>(seg->get_memory_size())});
        }
    }
    return loadableSegments_;
}

BadElf::BadElf(const std::string &msg) : runtime_error(msg) {}
const char *BadElf::what() const noexcept { return runtime_error::what(); }
} // namespace besm::elf
