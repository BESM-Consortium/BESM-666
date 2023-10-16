#include "besm-666/sim/plugin.hpp"

namespace besm::sim {

Plugin::Plugin(std::string const &path) : pluginLibrary_(path) {}
Plugin::Plugin(Plugin &&other)
    : pluginLibrary_(std::move(other.pluginLibrary_)) {}

Plugin const &Plugin::operator=(Plugin &&other) {
    std::swap(pluginLibrary_, other.pluginLibrary_);
    return *this;
}

void Plugin::init(sim::HookManager::SPtr hookManager,
                  std::string const &commandLine,
                  std::ostream &defaultLogStream) {
    pluginLibrary_.getFunction<InitFunction>("init")(hookManager, commandLine,
                                                     defaultLogStream);
}

} // namespace besm::sim
