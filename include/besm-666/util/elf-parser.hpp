#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "besm-666/riscv-types.hpp"
#include "besm-666/util/non-copyable.hpp"

namespace besm::util {

/**
 * \brief interface for elf parser, that can retrieve all LOAD segments
 */
class IElfParser : INonCopyable {
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
     * \brief stores all information about LOAD segments in vector and returns
     * a reference to it
     */
    virtual const std::vector<LoadableSegment> &getLoadableSegments() & = 0;

    virtual ~IElfParser() = default;
};

/**
 * \brief creates instance of specific ElfParser that is inheritor of
 * IElfParser
 */
std::unique_ptr<IElfParser> createParser(const std::filesystem::path &elfPath);

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
