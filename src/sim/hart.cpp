#include <cassert>
#include <iostream>
#include <utility>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"

namespace besm::sim {

Hart::SPtr Hart::Create(std::shared_ptr<mem::PhysMem> const &pMem,
                        std::shared_ptr<HookManager> const &hookManager) {
    return std::shared_ptr<Hart>(new Hart(pMem, hookManager));
}

Hart::Hart(std::shared_ptr<mem::PhysMem> const &pMem,
           std::shared_ptr<HookManager> hookManager)
    : mmu_(mem::MMU::Create(pMem)), dec_(mmu_), exec_(mmu_),
      hookManager_(std::move(hookManager)),
      prevPC_(std::numeric_limits<RV64UDWord>::max()), instrsExecuted_(0) {}

void Hart::runCycle() {
    RV64UDWord pc = exec_.getState().read(exec::GPRF::PC);
    assert(pc % 2 == 0);

    RV64Ptr startPC = pc;
    BasicBlock bb = dec_.parseBB(startPC);
    hookManager_->triggerHooks(HookManager::BASIC_BLOCK_PARSE, *this, &bb);
    exec_.execBB(bb);
    hookManager_->triggerHooks(HookManager::BASIC_BLOCK_EXECUTE, *this,
                               nullptr);
    // out-of-program control
    prevPC_ = startPC + bb.size() - 1;
    instrsExecuted_ += bb.size();

#ifdef false
    prevPC_ = pc;
    // fetch
    RV64UWord instrBytecode = dec_.fetch(pc);
    hookManager_->triggerHooks(HookManager::INSTRUCTION_FETCH, *this,
                               &instrBytecode);

    // decode
    Instruction instr = dec_.parse(instrBytecode);
    hookManager_->triggerHooks(HookManager::INSTRUCTION_DECODE, *this, &instr);

    // execute
    exec_.exec(instr);
    hookManager_->triggerHooks(HookManager::INSTRUCTION_EXECUTE, *this,
                               nullptr);
    ++instrsExecuted_;
#endif
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
