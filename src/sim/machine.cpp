#include "besm-666/sim/machine.hpp"

namespace besm::sim {

Machine::Machine(sim::Config const &config) {
    pMem_ = mem::PhysMemBuilder(4096, mem::PageAllocator::DefaultChunkSize)
                .loadElf(config.executablePath())
                .build();

    hookManager_ = sim::HookManager::Create();
    hart_ = sim::Hart::Create(pMem_, hookManager_);

    this->loadPlugins(config);
}

void Machine::run() { hart_->run(); }

exec::GPRF const &Machine::getState() const { return hart_->getState(); }

void Machine::loadPlugins(sim::Config const& config) {
    for(auto const& pluginCmd : config.plugins()) {
        std::filesystem::path pluginPath = pluginCmd.substr(0,
            pluginCmd.find(" "));

        Plugin plugin(pluginPath);
        plugin.init(hookManager_, pluginCmd, std::clog);
        plugins_.push_back(std::move(plugin));
    }
}

} // namespace besm::sim
