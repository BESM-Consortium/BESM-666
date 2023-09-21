#include "besm-666/util/elf-parser.hpp"

namespace besm::util {

ElfParser::ElfParser(const std::filesystem::path &elfPath) {
    if (!reader_.load(elfPath)) {
        throw InvalidELFFormat("Invalid ELF file: " + elfPath.string());
    }
    checkRequirements();
}
void ElfParser::checkRequirements() const {
    if (reader_.get_class() != Requirements::FileClass) {
        throw UnavailableELFRequirements("Wrong ELF file class.");
    }
    if (reader_.get_encoding() != Requirements::Encoding) {
        throw UnavailableELFRequirements("Wrong ELF encoding.");
    }
    if (reader_.get_machine() != Requirements::Arch) {
        throw UnavailableELFRequirements("Wrong ELF target architecture.");
    }
}
/**
 * @todo #9:15m/DEV to check that elf does not require dynamic linking
 */
const std::vector<ElfParser::LoadableSegment> &
ElfParser::getLoadableSegments() & {
    if (loadableSegments_.empty()) {
        for (const auto &seg : reader_.segments) {
            if (seg->get_type() == ELFIO::PT_LOAD) {
                loadableSegments_.emplace_back(
                    seg->get_virtual_address(), seg->get_data(),
                    static_cast<RV64Size>(seg->get_memory_size()));
            }
        }
    }
    return loadableSegments_;
}

ElfParser::LoadableSegment::LoadableSegment(RV64Ptr address, const void *data,
                                            RV64Size size)
    : address(address), data(data), size(size) {}
ElfParser::LoadableSegment::LoadableSegment(ElfParser::LoadableSegment &&other)
    : address(other.address), data(other.data), size(other.size) {
    std::swap(other.address, address);
    std::swap(other.data, data);
    std::swap(other.size, size);
}
ElfParser::LoadableSegment &
ElfParser::LoadableSegment::operator=(ElfParser::LoadableSegment &&other) {
    if (this != &other) {
        address = 0;
        data = nullptr;
        size = 0;
        std::swap(other.address, address);
        std::swap(other.data, data);
        std::swap(other.size, size);
    }
    return *this;
}

InvalidELFFormat::InvalidELFFormat(const char *msg) : runtime_error(msg) {}
InvalidELFFormat::InvalidELFFormat(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidELFFormat::what() const noexcept {
    return runtime_error::what();
}

UnavailableELFRequirements::UnavailableELFRequirements(const char *msg)
    : runtime_error(msg) {}
UnavailableELFRequirements::UnavailableELFRequirements(const std::string &msg)
    : runtime_error(msg) {}
const char *UnavailableELFRequirements::what() const noexcept {
    return runtime_error::what();
}
} // namespace besm::util
