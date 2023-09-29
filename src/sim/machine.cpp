#include "besm-666/sim/machine.hpp"

namespace besm::sim {

Machine::Machine(sim::Config const &config) {
    pMem_ = mem::PhysMemBuilder(4096, mem::PageAllocator::DefaultChunkSize)
                .loadElf(config.executablePath())
                .build();

    hart_ = sim::Hart::Create(pMem_);
}

void Machine::run() { hart_->run(); }

exec::GPRF const &Machine::getState() const { return hart_->getState(); }

} // namespace besm::sim
