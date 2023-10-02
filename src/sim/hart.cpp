#include <cassert>
#include <iostream>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"

namespace besm::sim {

Hart::SPtr Hart::Create(mem::PhysMem::SPtr pMem,
                        std::shared_ptr<HookManager> const &hookManager) {
    return std::shared_ptr<Hart>(new Hart(pMem, hookManager));
}

Hart::Hart(mem::PhysMem::SPtr pMem,
           std::shared_ptr<HookManager> const &hookManager)
    : mmu_(mem::MMU::Create(pMem)), exec_(mmu_), hookManager_(hookManager),
      prevPC_(std::numeric_limits<RV64UDWord>::max()) {}

void Hart::runCycle() {
    RV64UDWord pc = exec_.getState().read(exec::GPRF::PC);
    assert(pc % 2 == 0);

    // out-of-program control
    prevPC_ = pc;

    // fetch
    RV64UWord instrBytecode = mmu_->loadWord(pc);
    hookManager_->triggerHooks(HookManager::INSTRUCTION_FETCH, *this,
                               &instrBytecode);

    // decode
    Instruction instr = dec_.parse(instrBytecode);
    hookManager_->triggerHooks(HookManager::INSTRUCTION_DECODE, *this, &instr);

    // execute
    exec_.exec(instr);
    hookManager_->triggerHooks(HookManager::INSTRUCTION_EXECUTE, *this,
                               nullptr);
}

bool Hart::finished() const {
    return exec_.getState().read(exec::GPRF::PC) == prevPC_;
}

void Hart::run() {
    while (!this->finished()) {
        this->runCycle();
    }
}

} // namespace besm::sim
