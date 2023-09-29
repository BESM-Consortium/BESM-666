#include <cassert>

#include "besm-666/sim/hart.hpp"

namespace besm::sim {

Hart::Hart(mem::PhysMem::SPtr pMem)
    : mmu_(mem::MMU::Create(pMem)), exec_(mmu_),
      prevPC_(std::numeric_limits<RV64UDWord>::max()) {}

void Hart::runCycle() {
    RV64UDWord pc = exec_.getState().read(exec::GPRF::PC);
    assert(pc % 4 == 0);

    // fetch
    RV64UWord instrBytecode = mmu_->loadWord(pc);

    // decode
    Instruction instr = dec_.parse(instrBytecode);

    // execute
    exec_.exec(instr);

    // out-of-program control
    prevPC_ = pc;
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
