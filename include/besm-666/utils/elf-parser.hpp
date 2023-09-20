#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/utils/non-copyable.hpp"
#include "elfio/elfio.hpp"

namespace besm::utl {

class ElfParser final {
public:
    enum Requirements {
        /// 64bit
        FileClass = ELFIO::ELFCLASS64,
        /// little-endian
        Encoding = ELFIO::ELFDATA2LSB,
        /// RISC-V
        Arch = ELFIO::EM_RISCV
    };

    struct LoadableSegment: INonCopyable {
        RV64Ptr address;
        void const *data;
        RV64Size size;

        LoadableSegment(RV64Ptr address, void const *data, RV64Size size);
        LoadableSegment(LoadableSegment &&other);
        LoadableSegment &operator=(LoadableSegment &&other);
    };

    ElfParser(const std::filesystem::path &elfPath);

    const std::vector<LoadableSegment> &getLoadableSegments();

private:
    void checkRequirements();

    ELFIO::elfio reader_{};
    std::vector<LoadableSegment> loadableSegments_;
};

class BadElf : public std::runtime_error {
public:
    BadElf(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace besm::elf
