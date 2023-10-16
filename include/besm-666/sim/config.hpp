#pragma once

#include <cstddef>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace besm::sim {

struct ConfigData {
    // Input files
    std::filesystem::path executablePath;
    std::vector<std::string> plugins;
};

class InvalidConfiguration : public std::runtime_error {
public:
    InvalidConfiguration(std::string const &string)
        : std::runtime_error(string) {}
    InvalidConfiguration(char const *cstring) : std::runtime_error(cstring) {}
};

class Config {
public:
    std::filesystem::path executablePath() const;
    std::vector<std::string> const &plugins() const;

private:
    friend class ConfigBuilder;
    Config(ConfigData &&data) : data_(std::move(data)) {}

    ConfigData data_;
};

class ConfigBuilder {
public:
    ConfigBuilder() = default;

    void setExecutablePath(std::filesystem::path executablePath);
    void addPlugin(std::filesystem::path path);

    Config build();

private:
    void validateState() const;

    ConfigData data_;
};

} // namespace besm::sim
