#include "besm-666/sim/machine.hpp"
#include "besm-666/memory/phys-mem.hpp"

namespace besm::sim {

Machine::Machine(sim::Config const &config) {
    mem::PhysMemBuilder pMemBuilder;
    pMemBuilder.mapRAM(0, 1 * 1024 * 1024 * 1024, 4096, 2 * 1024 * 1024);
    pMem_ = pMemBuilder.build();

    mem::PhysMemLoader(pMem_).loadElf(config.executablePath());

    hookManager_ = sim::HookManager::Create();
    hart_ = sim::Hart::Create(pMem_, hookManager_);
}

void Machine::run() { hart_->run(); }

exec::GPRF const &Machine::getState() const { return hart_->getState(); }

} // namespace besm::sim
