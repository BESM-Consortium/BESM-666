#pragma once

#include "besm-666/basic-block.hpp"
#include "besm-666/decoder/decoder.hpp"
#include "besm-666/exec/csrf.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/util/assotiative-cache.hpp"

#include <memory>

namespace besm::sim {

template <typename PayloadType, typename TagType>
TagType BBTag(PayloadType const &payload) {
    return payload.startPC();
}

template <typename TagType, typename HashType>
HashType BBHash(TagType const &tag) {
    // Can be used: tag % sets, std::hash<TagType>{}(tag) % sets, (tag & 0x1FC)
    // >> 2. (tag & 0x1FC) >> 2 repeats hash every 128 byte aka num of set.
    // Value range of (tag & 0x1FC) >> 2 is [0, 128].

    return (tag & 0x1FC) >> 2;
}

class HookManager;

class Hart : public INonCopyable {
public:
    using SPtr = std::shared_ptr<Hart>;
    using BBCache =
        besm::util::Cache<BasicBlock, RV64UDWord, RV64UDWord, BBTag, BBHash>;
    static SPtr Create(std::shared_ptr<mem::PhysMem> const &pMem,
                       std::shared_ptr<HookManager> const &hookManager);

    void exec(Instruction const instr);
    void execBB(const BasicBlock &bb);

    exec::GPRF const &getGPRF() const { return gprf_; }
    exec::CSRF const &getCSRF() const { return csrf_; }
    mem::MMU const &getMMU() const { return *mmu_; }
    size_t getInstrsExecuted() const { return instrsExecuted_; }

    void runCycle();
    bool finished() const;

    void run();

private:
    explicit Hart(std::shared_ptr<mem::PhysMem> const &pMem,
                  std::shared_ptr<HookManager> hookManager);

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

    BBCache cache_;
    mem::MMU::SPtr mmu_;
    dec::Decoder dec_;
    exec::GPRF gprf_;
    exec::CSRF csrf_;
    std::shared_ptr<sim::HookManager> hookManager_;

    bool exceptionHappened_ = false;

    RV64UDWord prevPC_;
    size_t instrsExecuted_;
};

} // namespace besm::sim
