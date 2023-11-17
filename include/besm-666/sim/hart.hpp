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
    using Handler = void (Hart::*)();

    static SPtr Create(std::shared_ptr<mem::PhysMem> const &pMem,
                       std::shared_ptr<HookManager> const &hookManager);

#if true
    void exec(const Instruction instr);
    void execBB(const BasicBlock &bb);
    void runCycle();
#endif

    exec::GPRF const &getGPRF() const { return gprf_; }
    exec::CSRF const &getCSRF() const { return csrf_; }
    mem::MMU const &getMMU() const { return *mmu_; }
    size_t getInstrsExecuted() const { return instrsExecuted_; }

    bool finished() const;

    void run();

private:
    BBCache cache_;
    mem::MMU::SPtr mmu_;
    dec::Decoder dec_;
    exec::GPRF gprf_;
    exec::CSRF csrf_;
    BasicBlock *currentBB_;
    std::shared_ptr<sim::HookManager> hookManager_;

    bool exceptionHappened_ = false;

    RV64UDWord prevPC_;
    size_t instrsExecuted_;

    explicit Hart(std::shared_ptr<mem::PhysMem> const &pMem,
                  std::shared_ptr<HookManager> hookManager);

    void raiseException(ExceptionId id);
    void raiseIllegalInstruction();

    void exec_BB_END();
    void exec_INV_OP();
    void exec_ADDI();
    void exec_SLTI();
    void exec_SLTIU();
    void exec_ANDI();
    void exec_ORI();
    void exec_XORI();
    void exec_SLLI();
    void exec_SRLI();
    void exec_SRAI();
    void exec_LUI();
    void exec_AUIPC();
    void exec_ADD();
    void exec_SLT();
    void exec_SLTU();
    void exec_AND();
    void exec_OR();
    void exec_XOR();
    void exec_SLL();
    void exec_SRL();
    void exec_SUB();
    void exec_SRA();
    void exec_JAL();
    void exec_JALR();
    void exec_BEQ();
    void exec_BNE();
    void exec_BLT();
    void exec_BLTU();
    void exec_BGE();
    void exec_BGEU();

    void exec_LB();
    void exec_LH();
    void exec_LW();
    void exec_LD();
    void exec_LBU();
    void exec_LHU();
    void exec_LWU();
    void exec_SB();
    void exec_SH();
    void exec_SW();
    void exec_SD();

    // Does nothing in in-order implementation
    void exec_FENCE();
    void exec_FENCE_TSO();

    // todo: to be implemented
    void exec_PAUSE();

    // Will be implemented after CSR system release
    void exec_ECALL();
    void exec_EBREAK();

    void exec_ADDIW();
    void exec_SLLIW();
    void exec_SRLIW();
    void exec_SRAIW();
    void exec_ADDW();
    void exec_SUBW();
    void exec_SLLW();
    void exec_SRLW();
    void exec_SRAW();

    void exec_MRET();
    void exec_SRET();

    void exec_CSRRW();
    void exec_CSRRS();
    void exec_CSRRC();

    void exec_CSRRWI();
    void exec_CSRRSI();
    void exec_CSRRCI();

    void nextPC();

public:
    // handler index is enum value of operation (keep this invariant!!!)
    /**
     * @todo #39:90min Make autogen for rv-instruction-op.hpp header and this
     * array to keep them consistent (handler index is enum value of operation)
     */
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
        &Hart::exec_CSRRCI,    &Hart::exec_SRET,   &Hart::exec_MRET,
        &Hart::exec_BB_END};
};

} // namespace besm::sim
