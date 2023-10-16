#pragma once

#include <vector>

#include "besm-666/sim/config.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/sim/plugin.hpp"
#include "besm-666/util/non-copyable.hpp"

namespace besm::sim {

class Machine : INonCopyable {
public:
    Machine(sim::Config const &config);

    void run();

    exec::GPRF const &getState() const;

    size_t getInstrsExecuted() const { return hart_->getInstrsExecuted(); }
    sim::HookManager::SPtr getHookManager() { return hookManager_; }

private:
    void loadPlugins(sim::Config const &config);

    HookManager::SPtr hookManager_;
    std::vector<Plugin> plugins_;
    mem::PhysMem::SPtr pMem_;
    sim::Hart::SPtr hart_;
};

} // namespace besm::sim
