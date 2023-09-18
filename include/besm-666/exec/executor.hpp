#pragma once

#include "besm-666/instruction.hpp"
#include "besm-666/reg/rf.hpp"
#include "besm-666/rv-instruction-op.hpp"

namespace besm::exec {

class Executor {
public:
    Executor(reg::GPRF &gprf) : gprf_(gprf) {}

    void exec(Instruction instr);

private:
    void exec_ADDI(Instruction instr);
    void exec_SLTI(Instruction instr);
    void exec_SLTU(Instruction instr);
    void exec_ANDI(Instruction instr);
    void exec_ORI(Instruction instr);
    void exec_XORI(Instruction instr);
    void exec_LUI(Instruction instr);
    void exec_AUIPC(Instruction instr);

    reg::GPRF &gprf_;
};

} // namespace besm::exec
