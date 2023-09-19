#pragma once

#include "besm-666/instruction.hpp"
#include "besm-666/reg/rf.hpp"
#include "besm-666/rv-instruction-op.hpp"

namespace besm::exec {

class Executor {
public:
    Executor() = default;

    void exec(Instruction instr);

    inline reg::GPRF const &getState() const { return gprf_; }

private:
    void exec_ADDI(Instruction instr);
    void exec_SLTI(Instruction instr);
    void exec_SLTIU(Instruction instr);
    void exec_ANDI(Instruction instr);
    void exec_ORI(Instruction instr);
    void exec_XORI(Instruction instr);
    void exec_SLLI(Instruction instr);
    void exec_SRLI(Instruction instr);
    void exec_SRAI(Instruction instr);
    void exec_LUI(Instruction instr);
    void exec_AUIPC(Instruction instr);
    void exec_ADD(Instruction instr);
    void exec_SLT(Instruction instr);
    void exec_SLTU(Instruction instr);
    void exec_AND(Instruction instr);
    void exec_OR(Instruction instr);
    void exec_XOR(Instruction instr);
    void exec_SLL(Instruction instr);
    void exec_SRL(Instruction instr);
    void exec_SUB(Instruction instr);
    void exec_SRA(Instruction instr);
    void exec_JAL(Instruction instr);
    void exec_JALR(Instruction instr);
    void exec_BEQ(Instruction instr);
    void exec_BNE(Instruction instr);
    void exec_BLT(Instruction instr);
    void exec_BLTU(Instruction instr);
    void exec_BGE(Instruction instr);
    void exec_BGEU(Instruction instr);

    // Will be implemented after memory system release
    void exec_LB(Instruction instr) {}
    void exec_LH(Instruction instr) {}
    void exec_LW(Instruction instr) {}
    void exec_LBU(Instruction instr) {}
    void exec_LHU(Instruction instr) {}
    void exec_LWU(Instruction instr) {}
    void exec_SB(Instruction instr) {}
    void exec_SH(Instruction instr) {}
    void exec_SW(Instruction instr) {}

    // Does nothing in in-order implementation
    void exec_FENCE(Instruction instr) { this->nextPC(); }
    void exec_FENCE_TSO(Instruction instr) { this->nextPC(); }

    // Will be implemented after CSR system release
    void exec_ECALL(Instruction instr) { this->nextPC(); }
    void exec_EBREAK(Instruction instr) { this->nextPC(); }

    void nextPC();

    reg::GPRF gprf_;
};

} // namespace besm::exec
