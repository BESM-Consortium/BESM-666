#pragma once

#include "besm-666/sim/config.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/util/non-copyable.hpp"

namespace besm::sim {

class Machine : INonCopyable {
public:
    Machine(sim::Config const &config);

    void run();

    exec::GPRF const &getState() const;
    sim::HookManager::SPtr getHookManager() { return hookManager_; }

private:
    HookManager::SPtr hookManager_;
    mem::PhysMem::SPtr pMem_;
    sim::Hart::SPtr hart_;
};

} // namespace besm::sim
