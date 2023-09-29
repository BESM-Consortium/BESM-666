#pragma once

#include "besm-666/decoder/decoder.hpp"
#include "besm-666/exec/executor.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"

namespace besm::sim {

class Hart {
public:
    explicit Hart(mem::PhysMem::SPtr pMem);

    exec::GPRF const &getState() const { return exec_.getState(); }

    void runCycle();
    bool finished() const;

    void run();

private:
    dec::Decoder dec_;
    mem::MMU::SPtr mmu_;
    exec::Executor exec_;

    RV64UDWord prevPC_;
};

} // namespace besm::sim
