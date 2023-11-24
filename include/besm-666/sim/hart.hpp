#pragma once

#include "besm-666/decoder/decoder.hpp"
#include "besm-666/exec/basic-block.hpp"
#include "besm-666/exec/csrf.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/memory/prefetcher.hpp"
#include "besm-666/util/assotiative-cache.hpp"

#include <memory>

namespace besm::sim {

class HookManager;

class Hart : public INonCopyable {
public:
    using SPtr = std::shared_ptr<Hart>;
    using Handler = void (*)(Hart &);

    static SPtr Create(std::shared_ptr<mem::PhysMem> const &pMem,
                       std::shared_ptr<HookManager> const &hookManager);

    exec::GPRF const &getGPRF() const { return gprf_; }
    exec::CSRF const &getCSRF() const { return csrf_; }
    mem::MMU const &getMMU() const { return *mmu_; }
    size_t getInstrsExecuted() const { return instrsExecuted_; }

    bool finished() const;

    void run();

private:
    exec::BasicBlockCache bbCache_;
    Instruction const *currentInstr_;

    dec::Decoder dec_;

    mem::MMU::SPtr mmu_;
    mem::Prefetcher prefetcher_;

    exec::GPRF gprf_;
    exec::CSRF csrf_;

    std::shared_ptr<sim::HookManager> hookManager_;

    size_t instrsExecuted_;

    explicit Hart(std::shared_ptr<mem::PhysMem> const &pMem,
                  std::shared_ptr<HookManager> hookManager);

    void assembleBB(exec::BasicBlock &bb, RV64Ptr pc);
    void fetchBB();
    inline static void execNextInstr(Hart &hart);

    void raiseException(ExceptionId id);
    void raiseIllegalInstruction();

    static void exec_BB_END(Hart &hart);
    static void exec_INV_OP(Hart &hart);
    static void exec_ADDI(Hart &hart);
    static void exec_SLTI(Hart &hart);
    static void exec_SLTIU(Hart &hart);
    static void exec_ANDI(Hart &hart);
    static void exec_ORI(Hart &hart);
    static void exec_XORI(Hart &hart);
    static void exec_SLLI(Hart &hart);
    static void exec_SRLI(Hart &hart);
    static void exec_SRAI(Hart &hart);
    static void exec_LUI(Hart &hart);
    static void exec_AUIPC(Hart &hart);
    static void exec_ADD(Hart &hart);
    static void exec_SLT(Hart &hart);
    static void exec_SLTU(Hart &hart);
    static void exec_AND(Hart &hart);
    static void exec_OR(Hart &hart);
    static void exec_XOR(Hart &hart);
    static void exec_SLL(Hart &hart);
    static void exec_SRL(Hart &hart);
    static void exec_SUB(Hart &hart);
    static void exec_SRA(Hart &hart);
    static void exec_JAL(Hart &hart);
    static void exec_JALR(Hart &hart);
    static void exec_BEQ(Hart &hart);
    static void exec_BNE(Hart &hart);
    static void exec_BLT(Hart &hart);
    static void exec_BLTU(Hart &hart);
    static void exec_BGE(Hart &hart);
    static void exec_BGEU(Hart &hart);

    static void exec_LB(Hart &hart);
    static void exec_LH(Hart &hart);
    static void exec_LW(Hart &hart);
    static void exec_LD(Hart &hart);
    static void exec_LBU(Hart &hart);
    static void exec_LHU(Hart &hart);
    static void exec_LWU(Hart &hart);
    static void exec_SB(Hart &hart);
    static void exec_SH(Hart &hart);
    static void exec_SW(Hart &hart);
    static void exec_SD(Hart &hart);

    // Does nothing in in-order implementation
    static void exec_FENCE(Hart &hart);
    static void exec_FENCE_TSO(Hart &hart);

    /**
     * @todo #81:90min to be implemented
     */
    static void exec_PAUSE(Hart &hart);

    // Will be implemented after CSR system release
    static void exec_ECALL(Hart &hart);
    static void exec_EBREAK(Hart &hart);

    static void exec_ADDIW(Hart &hart);
    static void exec_SLLIW(Hart &hart);
    static void exec_SRLIW(Hart &hart);
    static void exec_SRAIW(Hart &hart);
    static void exec_ADDW(Hart &hart);
    static void exec_SUBW(Hart &hart);
    static void exec_SLLW(Hart &hart);
    static void exec_SRLW(Hart &hart);
    static void exec_SRAW(Hart &hart);

    static void exec_MRET(Hart &hart);
    static void exec_SRET(Hart &hart);

    static void exec_CSRRW(Hart &hart);
    static void exec_CSRRS(Hart &hart);
    static void exec_CSRRC(Hart &hart);

    static void exec_CSRRWI(Hart &hart);
    static void exec_CSRRSI(Hart &hart);
    static void exec_CSRRCI(Hart &hart);

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
