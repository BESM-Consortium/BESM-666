#pragma once

#include <ostream>
#include <string>

#include "besm-666/sim/hooks.hpp"
#include "besm-666/util/dynamic-library.hpp"
#include "besm-666/util/non-copyable.hpp"

namespace besm::sim {

class Plugin : public INonCopyable {
public:
    Plugin(std::string const &path);
    Plugin(Plugin &&other);

    Plugin const &operator=(Plugin &&other);

    ~Plugin() = default;

    void init(sim::HookManager::SPtr hookManager,
              std::string const &commandLine, std::ostream &defaulLogStream);

private:
    using InitFunction = void (*)(sim::HookManager::SPtr, std::string const &,
                                  std::ostream &);

    util::DynamicLibrary pluginLibrary_;
};

} // namespace besm::sim
