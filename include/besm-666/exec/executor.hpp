#pragma once

#include "besm-666/basic-block.hpp"
#include "besm-666/exec/csrf.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/instruction.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/rv-instruction-op.hpp"
#include "besm-666/sim/hooks.hpp"

namespace besm::exec {

class Executor {
public:
    explicit Executor(mem::MMU::SPtr mmu, sim::HookManager::SPtr hookManager)
        : mmu_(std::move(mmu)), hookManager_(hookManager) {}

    void exec(Instruction const instr);
    void execBB(const BasicBlock &bb);

    inline GPRF const &getState() const { return gprf_; }
    inline CSRF const &getCSRF() const { return csrf_; }

private:
    void raiseException(ExceptionId id);
    void raiseIllegalInstruction();

    void exec_ADDI(Instruction const instr);
    void exec_SLTI(Instruction const instr);
    void exec_SLTIU(Instruction const instr);
    void exec_ANDI(Instruction const instr);
    void exec_ORI(Instruction const instr);
    void exec_XORI(Instruction const instr);
    void exec_SLLI(Instruction const instr);
    void exec_SRLI(Instruction const instr);
    void exec_SRAI(Instruction const instr);
    void exec_LUI(Instruction const instr);
    void exec_AUIPC(Instruction const instr);
    void exec_ADD(Instruction const instr);
    void exec_SLT(Instruction const instr);
    void exec_SLTU(Instruction const instr);
    void exec_AND(Instruction const instr);
    void exec_OR(Instruction const instr);
    void exec_XOR(Instruction const instr);
    void exec_SLL(Instruction const instr);
    void exec_SRL(Instruction const instr);
    void exec_SUB(Instruction const instr);
    void exec_SRA(Instruction const instr);
    void exec_JAL(Instruction const instr);
    void exec_JALR(Instruction const instr);
    void exec_BEQ(Instruction const instr);
    void exec_BNE(Instruction const instr);
    void exec_BLT(Instruction const instr);
    void exec_BLTU(Instruction const instr);
    void exec_BGE(Instruction const instr);
    void exec_BGEU(Instruction const instr);

    void exec_LB(Instruction const instr);
    void exec_LH(Instruction const instr);
    void exec_LW(Instruction const instr);
    void exec_LD(Instruction const instr);
    void exec_LBU(Instruction const instr);
    void exec_LHU(Instruction const instr);
    void exec_LWU(Instruction const instr);
    void exec_SB(Instruction const instr);
    void exec_SH(Instruction const instr);
    void exec_SW(Instruction const instr);
    void exec_SD(Instruction const instr);

    // Does nothing in in-order implementation
    void exec_FENCE(Instruction const instr) { this->nextPC(); }
    void exec_FENCE_TSO(Instruction const instr) { this->nextPC(); }

    // Will be implemented after CSR system release
    void exec_ECALL(Instruction const instr);
    void exec_EBREAK(Instruction const instr);

    void exec_ADDIW(Instruction const instr);
    void exec_SLLIW(Instruction const instr);
    void exec_SRLIW(Instruction const instr);
    void exec_SRAIW(Instruction const instr);
    void exec_ADDW(Instruction const instr);
    void exec_SUBW(Instruction const instr);
    void exec_SLLW(Instruction const instr);
    void exec_SRLW(Instruction const instr);
    void exec_SRAW(Instruction const instr);

    void exec_MRET(Instruction const instr);
    void exec_SRET(Instruction const instr);

    void exec_CSRRW(Instruction const instr);
    void exec_CSRRS(Instruction const instr);
    void exec_CSRRC(Instruction const instr);

    void exec_CSRRWI(Instruction const instr);
    void exec_CSRRSI(Instruction const instr);
    void exec_CSRRCI(Instruction const instr);

    void nextPC();

    GPRF gprf_;
    CSRF csrf_;
    mem::MMU::SPtr mmu_;
    sim::HookManager::SPtr hookManager_;

    bool exceptionHappened_ = false;
};

} // namespace besm::exec
