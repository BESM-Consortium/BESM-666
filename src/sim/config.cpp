#include "besm-666/sim/config.hpp"

namespace besm::sim {
std::filesystem::path Config::executablePath() const {
    return data_.executablePath;
}

void ConfigBuilder::setExecutablePath(std::filesystem::path path) {
    data_.executablePath = path;
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
