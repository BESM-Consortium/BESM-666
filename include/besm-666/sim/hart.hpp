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
    using Handler = void (Hart::*)(Instruction const);

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
    BBCache cache_;
    mem::MMU::SPtr mmu_;
    dec::Decoder dec_;
    exec::GPRF gprf_;
    exec::CSRF csrf_;
    BasicBlock currentBB_;
    std::shared_ptr<sim::HookManager> hookManager_;

    bool exceptionHappened_ = false;

    RV64UDWord prevPC_;
    size_t instrsExecuted_;

    explicit Hart(std::shared_ptr<mem::PhysMem> const &pMem,
                  std::shared_ptr<HookManager> hookManager);

    void raiseException(ExceptionId id);
    void raiseIllegalInstruction();

    void exec_INV_OP(Instruction const instr);
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

    // todo: to be implemented
    void exec_PAUSE(Instruction const instr) {
        this->raiseIllegalInstruction();
    }

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

public:
    static constexpr Handler HANDLER_ARR[] = {
        &Hart::exec_INV_OP,    &Hart::exec_LUI,    &Hart::exec_AUIPC,
        &Hart::exec_JAL,       &Hart::exec_JALR,   &Hart::exec_BEQ,
        &Hart::exec_BNE,       &Hart::exec_BLT,    &Hart::exec_BGE,
        &Hart::exec_BLTU,      &Hart::exec_BGEU,   &Hart::exec_LB,
        &Hart::exec_LH,        &Hart::exec_LW,     &Hart::exec_LBU,
        &Hart::exec_LHU,       &Hart::exec_SB,     &Hart::exec_SH,
        &Hart::exec_SW,        &Hart::exec_ADDI,   &Hart::exec_SLTI,
        &Hart::exec_SLTIU,     &Hart::exec_XORI,   &Hart::exec_ORI,
        &Hart::exec_ANDI,      &Hart::exec_ADD,    &Hart::exec_SUB,
        &Hart::exec_SLL,       &Hart::exec_SLT,    &Hart::exec_SLTU,
        &Hart::exec_XOR,       &Hart::exec_SRL,    &Hart::exec_SRA,
        &Hart::exec_OR,        &Hart::exec_AND,    &Hart::exec_FENCE,
        &Hart::exec_FENCE_TSO, &Hart::exec_PAUSE,  &Hart::exec_ECALL,
        &Hart::exec_EBREAK,    &Hart::exec_LWU,    &Hart::exec_LD,
        &Hart::exec_SD,        &Hart::exec_SLLI,   &Hart::exec_SRLI,
        &Hart::exec_SRAI,      &Hart::exec_ADDIW,  &Hart::exec_SLLIW,
        &Hart::exec_SRLIW,     &Hart::exec_SRAIW,  &Hart::exec_ADDW,
        &Hart::exec_SUBW,      &Hart::exec_SLLW,   &Hart::exec_SRLW,
        &Hart::exec_SRAW,      &Hart::exec_CSRRW,  &Hart::exec_CSRRS,
        &Hart::exec_CSRRC,     &Hart::exec_CSRRWI, &Hart::exec_CSRRSI,
        &Hart::exec_CSRRCI,    &Hart::exec_SRET,   &Hart::exec_MRET};
};

} // namespace besm::sim
