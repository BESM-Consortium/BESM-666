#pragma once

#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/config.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/util/non-copyable.hpp"

namespace besm::sim {

class Machine : INonCopyable {
public:
    Machine(sim::Config const &config);

    void run();

    sim::Hart const &getHart() const;

    size_t getInstrsExecuted() const { return hart_->getInstrsExecuted(); }
    sim::HookManager &getHookManager() { return *hookManager_; }

private:
    HookManager::SPtr hookManager_;
    std::shared_ptr<mem::PhysMem> pMem_;
    sim::Hart::SPtr hart_;
};

} // namespace besm::sim
