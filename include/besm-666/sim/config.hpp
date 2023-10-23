#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <vector>

#include "besm-666/riscv-types.hpp"
#include "besm-666/util/range.hpp"

namespace besm::sim {

struct ConfigData {
    // Input files
    std::filesystem::path executablePath;

    // Memory
    std::vector<util::Range<RV64Ptr>> ramRanges;
    size_t ramPageSize;
    size_t ramChunkSize;
};

class InvalidConfiguration : public std::runtime_error {
public:
    InvalidConfiguration(std::string const &string)
        : std::runtime_error(string) {}
    InvalidConfiguration(char const *cstring) : std::runtime_error(cstring) {}
};

class Config {
public:
    Config();

    std::filesystem::path executablePath() const;
    std::vector<util::Range<RV64Ptr>> const &ramRanges() const;
    size_t ramPageSize() const;
    size_t ramChunkSize() const;

private:
    friend class ConfigBuilder;
    Config(ConfigData &&data) : data_(std::move(data)) {}

    ConfigData data_;
};

class ConfigBuilder {
public:
    ConfigBuilder() = default;

    void setExecutablePath(std::filesystem::path executablePath);
    void addRamRange(util::Range<RV64Ptr> range);
    void setRamPageSize(size_t pageSize);
    void setRamChunkSize(size_t chunkSize);

    Config build();

private:
    void validateState() const;

    ConfigData data_;
};

} // namespace besm::sim
