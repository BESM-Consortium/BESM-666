#include "besm-666/sim/config.hpp"

namespace besm::sim {

Config::Config() {
    data_.ramPageSize = 0;
    data_.ramChunkSize = 0;
}

std::filesystem::path Config::executablePath() const {
    return data_.executablePath;
}

std::vector<util::Range<RV64Ptr>> const &Config::ramRanges() const {
    return data_.ramRanges;
}

size_t Config::ramPageSize() const { return data_.ramPageSize; }
size_t Config::ramChunkSize() const { return data_.ramChunkSize; }

void ConfigBuilder::setExecutablePath(std::filesystem::path path) {
    data_.executablePath = path;
}

void ConfigBuilder::addRamRange(util::Range<RV64Ptr> range) {
    data_.ramRanges.push_back(range);
}
void ConfigBuilder::setRamPageSize(size_t pageSize) {
    data_.ramPageSize = pageSize;
}
void ConfigBuilder::setRamChunkSize(size_t chunkSize) {
    data_.ramChunkSize = chunkSize;
}

Config ConfigBuilder::build() {
    this->validateState();
    return Config(std::move(data_));
}

void ConfigBuilder::validateState() const {
    if (data_.executablePath.empty()) {
        throw InvalidConfiguration("Invalid executable path");
    }
}
} // namespace besm::sim
