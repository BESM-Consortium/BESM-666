#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/non-copyable.hpp"
#include "elfio/elfio.hpp"

namespace besm::util {

class ElfParser final : INonCopyable {
public:
    struct LoadableSegment : INonCopyable {
        RV64Ptr address;
        const void *data;
        RV64Size size;

        LoadableSegment(RV64Ptr address, void const *data, RV64Size size);
        LoadableSegment(LoadableSegment &&other);
        LoadableSegment &operator=(LoadableSegment &&other);
    };

    ElfParser(const std::filesystem::path &elfPath);

    const std::vector<LoadableSegment> &getLoadableSegments() &;

private:
    enum Requirements {
        /// 64bit
        FileClass = ELFIO::ELFCLASS64,
        /// little-endian
        Encoding = ELFIO::ELFDATA2LSB,
        /// RISC-V
        Arch = ELFIO::EM_RISCV
    };

    void checkRequirements() const;

    ELFIO::elfio reader_{};
    std::vector<LoadableSegment> loadableSegments_;
};

class InvalidELFFormat : public std::runtime_error {
public:
    InvalidELFFormat(char const *msg);
    InvalidELFFormat(const std::string &msg);
    const char *what() const noexcept override;
};

class UnavailableELFRequirements : public std::runtime_error {
public:
    UnavailableELFRequirements(char const *msg);
    UnavailableELFRequirements(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace besm::util
