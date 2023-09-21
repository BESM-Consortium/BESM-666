#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/non-copyable.hpp"
#include "elfio/elfio.hpp"

namespace besm::util {

/**
 * \brief checks that ELF suites all simulator requirements and  retrieves all
 * LOAD segments
 */
class ElfParser final : INonCopyable {
public:
    /**
     * \brief stores all required data about segment to load
     */
    struct LoadableSegment : INonCopyable {
        RV64Ptr address;
        const void *data;
        RV64Size size;

        LoadableSegment(RV64Ptr address, void const *data, RV64Size size);
        LoadableSegment(LoadableSegment &&other);
        LoadableSegment &operator=(LoadableSegment &&other);
    };

    /**
     * \throws InvalidELFFormat if ELF format is invalid
     */
    ElfParser(const std::filesystem::path &elfPath);

    /**
     * \brief stores all information about LOAD segments in vector and returns
     * a reference to it
     */
    const std::vector<LoadableSegment> &getLoadableSegments() &;

private:
    /**
     * \brief stores ELF requirements for simulator
     */
    enum Requirements {
        FileClass = ELFIO::ELFCLASS64, ///< ELF file class (64bit)
        Encoding = ELFIO::ELFDATA2LSB, ///< little-endian
        Arch = ELFIO::EM_RISCV         ///< architecture (RISC-V)
    };

    /**
     * \brief checks that ELF suites all requirements
     * \throws UnavailableELFRequirements if ELF doesn't suite requirements
     */
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
