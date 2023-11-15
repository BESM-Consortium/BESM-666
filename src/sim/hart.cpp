#include <cassert>
#include <iostream>
#include <utility>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/util/assotiative-cache.hpp"

namespace besm::sim {

Hart::SPtr Hart::Create(std::shared_ptr<mem::PhysMem> const &pMem,
                        std::shared_ptr<HookManager> const &hookManager) {
    return std::shared_ptr<Hart>(new Hart(pMem, hookManager));
}

Hart::Hart(std::shared_ptr<mem::PhysMem> const &pMem,
           std::shared_ptr<HookManager> hookManager)
    : mmu_(mem::MMU::Create(pMem)), dec_(mmu_), exec_(mmu_, hookManager),
      hookManager_(std::move(hookManager)),
      prevPC_(std::numeric_limits<RV64UDWord>::max()), instrsExecuted_(0),
      cache_(4, 128) {}

void Hart::runCycle() {
    RV64UDWord pc = exec_.getState().read(exec::GPRF::PC);
    assert(pc % 2 == 0);

    auto &entry = cache_.find(pc);
    BasicBlock &bb = entry.getPayload();

    if (entry.valid() && entry.getTag() == pc) {
        exec_.execBB(bb);
    } else {
        /*
         * @todo #82:30m remove this branch by adding another functionality to
         * cache
         */

        if (entry.valid())
            bb.resetBB();

        bb.setStartPC(pc);
        dec_.assembleBB(bb);
        entry.setPayload(bb, pc);

        // It can give +2 mips average but need validate() and setTag()
        // to be implemented

        // entry.setTag(pc);
        // entry.validate();

        cache_.incCounter(pc);
        exec_.execBB(bb);
    }

    hookManager_->triggerBBFetchHook(bb);

    // out-of-program control
    prevPC_ = pc + bb.size() - 1;
    instrsExecuted_ += bb.size();
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
