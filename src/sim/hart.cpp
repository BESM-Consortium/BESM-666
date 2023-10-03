#include <cassert>
#include <iostream>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"

namespace besm::sim {

Hart::SPtr Hart::Create(mem::PhysMem::SPtr pMem) {
    return std::shared_ptr<Hart>(new Hart(pMem));
}

Hart::Hart(mem::PhysMem::SPtr pMem)
    : mmu_(mem::MMU::Create(pMem)), exec_(mmu_),
      prevPC_(std::numeric_limits<RV64UDWord>::max()) {}

void Hart::runCycle() {
    RV64UDWord pc = exec_.getState().read(exec::GPRF::PC);
    assert(pc % 2 == 0);

    // out-of-program control
    prevPC_ = pc;

    // fetch
    RV64UWord instrBytecode = mmu_->loadWord(pc);

    // decode
    Instruction instr = dec_.parse(instrBytecode);
    std::clog << "[HART] Fetched instruction " << instr.operation << std::endl;

    // execute
    exec_.exec(instr);

    exec::GPRFStateDumper(std::clog).dump(exec_.getState());
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
