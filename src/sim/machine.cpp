#include "besm-666/sim/machine.hpp"
#include "besm-666/memory/phys-mem.hpp"

namespace besm::sim {

Machine::Machine(sim::Config const &config) {
    std::clog << "[BESM] Creating memory system..." << std::endl;
    mem::PhysMemBuilder pMemBuilder;

    std::clog << "[BESM] MEMORY: RAM page size is " << config.ramPageSize()
              << std::endl;
    std::clog << "[BESM] MEMORY: RAM chunk size is " << config.ramChunkSize()
              << std::endl;
    for (auto range : config.ramRanges()) {
        std::clog << "[BESM] MEMORY: Added RAM range [" << range.leftBorder()
                  << ", " << range.size() << ")" << std::endl;
        pMemBuilder.mapRAM(range.leftBorder(), range.size(),
                           config.ramPageSize(), config.ramChunkSize());
    }

    pMem_ = pMemBuilder.build();

    mem::PhysMemLoader(pMem_).loadElf(config.executablePath());

    hookManager_ = sim::HookManager::Create();
    hart_ = sim::Hart::Create(pMem_, hookManager_);
}

void Machine::run() { hart_->run(); }

sim::Hart const &Machine::getHart() const { return *hart_; }

} // namespace besm::sim
