#include <cassert>
#include <iostream>
#include <utility>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/util/assotiative-cache.hpp"

namespace besm::sim {

Hart::SPtr Hart::Create(std::shared_ptr<mem::PhysMem> const &pMem,
                        std::shared_ptr<HookManager> const &hookManager) {
    return std::shared_ptr<Hart>(new Hart(pMem, hookManager));
}

Hart::Hart(std::shared_ptr<mem::PhysMem> const &pMem,
           std::shared_ptr<HookManager> hookManager)
    : mmu_(mem::MMU::Create(pMem)), dec_(mmu_),
      hookManager_(std::move(hookManager)),
      prevPC_(std::numeric_limits<RV64UDWord>::max()), instrsExecuted_(0),
      cache_(4, 128),
      currentBB_(nullptr) {}

#if true
void Hart::exec(const Instruction instr) {
    switch (instr.operation) {
    // RV32I
    case ADDI:
        exec_ADDI();
        break;
    case SLTI:
        exec_SLTI();
        break;
    case SLTIU:
        exec_SLTIU();
        break;
    case ANDI:
        exec_ANDI();
        break;
    case ORI:
        exec_ORI();
        break;
    case XORI:
        exec_XORI();
        break;
    case SLLI:
        exec_SLLI();
        break;
    case SRLI:
        exec_SRLI();
        break;
    case SRAI:
        exec_SRAI();
        break;
    case LUI:
        exec_LUI();
        break;
    case AUIPC:
        exec_AUIPC();
        break;
    case ADD:
        exec_ADD();
        break;
    case SLT:
        exec_SLT();
        break;
    case SLTU:
        exec_SLTU();
        break;
    case AND:
        exec_AND();
        break;
    case OR:
        exec_OR();
        break;
    case XOR:
        exec_XOR();
        break;
    case SLL:
        exec_SLL();
        break;
    case SRL:
        exec_SRL();
        break;
    case SUB:
        exec_SUB();
        break;
    case SRA:
        exec_SRA();
        break;
    case JAL:
        exec_JAL();
        break;
    case JALR:
        exec_JALR();
        break;
    case BEQ:
        exec_BEQ();
        break;
    case BNE:
        exec_BNE();
        break;
    case BLT:
        exec_BLT();
        break;
    case BLTU:
        exec_BLTU();
        break;
    case BGE:
        exec_BGE();
        break;
    case BGEU:
        exec_BGEU();
        break;
    case LB:
        exec_LB();
        break;
    case LH:
        exec_LH();
        break;
    case LW:
        exec_LW();
        break;
    case LD:
        exec_LD();
        break;
    case LBU:
        exec_LBU();
        break;
    case LHU:
        exec_LHU();
        break;
    case LWU:
        exec_LWU();
        break;
    case SB:
        exec_SB();
        break;
    case SH:
        exec_SH();
        break;
    case SW:
        exec_SW();
        break;
    case SD:
        exec_SW();
        break;
    case FENCE:
        exec_FENCE();
        break;
    case FENCE_TSO:
        exec_FENCE_TSO();
        break;
    case ECALL:
        exec_ECALL();
        break;
    case EBREAK:
        exec_EBREAK();
        break;
    case ADDIW:
        exec_ADDIW();
        break;
    case SLLIW:
        exec_SLLIW();
        break;
    case SRLIW:
        exec_SRLIW();
        break;
    case SRAIW:
        exec_SRAIW();
        break;
    case ADDW:
        exec_ADDW();
        break;
    case SUBW:
        exec_SUBW();
        break;
    case SLLW:
        exec_SLLW();
        break;
    case SRLW:
        exec_SRLW();
        break;
    case SRAW:
        exec_SRAW();
        break;
    case MRET:
        exec_MRET();
        break;
    case SRET:
        exec_SRET();
        break;
    case CSRRW:
        exec_CSRRW();
        break;
    case CSRRS:
        exec_CSRRS();
        break;
    case CSRRC:
        exec_CSRRC();
        break;
    case CSRRWI:
        exec_CSRRWI();
        break;
    case CSRRSI:
        exec_CSRRSI();
        break;
    case CSRRCI:
        exec_CSRRCI();
        break;
    default:
        this->raiseIllegalInstruction();
        break;
    }
}
void Hart::execBB(const BasicBlock &bb) {
    size_t instrIt = bb.currentInstr();
    while (instrIt != bb.size()) {
        exec(bb[instrIt]);
        hookManager_->triggerInstrExecHook(bb[instrIt]);
        if (exceptionHappened_) {
            exceptionHappened_ = false;
            break;
        }

        instrIt = bb.nextInstr();
    }
}
void Hart::runCycle() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    assert(pc % 2 == 0);

    auto &entry = cache_.find(pc);
    BasicBlock &bb = entry.getPayload();

    if (entry.valid() && entry.getTag() == pc) {
        execBB(bb);
    } else {
        /*
         * @todo #82:30m remove this branch by adding another functionality to
         * cache
         */

        if (entry.valid())
            bb.resetBB();

        bb.setStartPC(pc);
        dec_.assembleBB(bb);
        entry.setPayload(bb, pc);

        // It can give +2 mips average but need validate() and setTag()
        // to be implemented

        // entry.setTag(pc);
        // entry.validate();

        cache_.incCounter(pc);
        execBB(bb);
    }

    hookManager_->triggerBBFetchHook(bb);

    // out-of-program control
    prevPC_ = pc + bb.size() - 1;
    instrsExecuted_ += bb.size();
}
#endif

bool Hart::finished() const { return gprf_.read(exec::GPRF::PC) == prevPC_; }

void Hart::run() {
    static_assert(sizeof(sim::Hart::HANDLER_ARR) / sizeof(sim::Hart::Handler) ==
                  (InstructionOp::BB_END + 1));

    // TODO: may be macros?
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    assert(pc % 2 == 0);
    prevPC_ = pc;

    auto &entry = cache_.find(pc);
    currentBB_ = &entry.getPayload();
    currentBB_->resetBB();
    currentBB_->setStartPC(pc);
    dec_.assembleBB(*currentBB_);
    entry.setPayload(*currentBB_, pc);
    cache_.incCounter(pc);
    hookManager_->triggerBBFetchHook(*currentBB_);

    size_t instrIt = currentBB_->currentInstr();
    if (instrIt != currentBB_->size()) {
        (this->*HANDLER_ARR[(*currentBB_)[instrIt].operation])();
    }
}

void Hart::raiseException(ExceptionId id) {
    csrf_.mstatus.set<exec::MStatus::MPIE>(
        csrf_.mstatus.get<exec::MStatus::MIE>());
    csrf_.mstatus.set<exec::MStatus::MIE>(0);
    csrf_.mstatus.set<exec::MStatus::MPP>(csrf_.getPrivillege());
    csrf_.setPrivillege(exec::PRIVILLEGE_MACHINE);

    csrf_.mcause.set<exec::MCause::Interrupt>(0);
    csrf_.mcause.set<exec::MCause::ExceptionCode>(id);

    RV64Ptr newPC;
    if (csrf_.mtvec.get<exec::MTVec::Mode>() == exec::MTVec::VectoredMode) {
        newPC = (csrf_.mtvec.get<exec::MTVec::Base>() + id) * 4;
    } else {
        newPC = csrf_.mtvec.get<exec::MTVec::Base>() * 4;
    }

    csrf_.mepc.set<exec::MEPC::Value>(gprf_.read(exec::GPRF::PC));
    gprf_.write(exec::GPRF::PC, newPC);

    exceptionHappened_ = true;
}
void Hart::raiseIllegalInstruction() {
    this->raiseException(EXCEPTION_ILLEGAL_INSTR);
}

void Hart::exec_BB_END() {
    // TODO: check finished
    prevPC_ = currentBB_->startPC() + (currentBB_->size() - 2) * sizeof(RV64UWord);
    instrsExecuted_ += currentBB_->size() - 1;
    if(this->finished()) {
        return;
    }
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    assert(pc % 2 == 0);
    auto &entry = cache_.find(pc);
    currentBB_ = &entry.getPayload();
    if (!entry.valid() || entry.getTag() != pc) {
        if (entry.valid()) {
            currentBB_->resetBB();
        }
        currentBB_->setStartPC(pc);
        dec_.assembleBB(*currentBB_);
        entry.setPayload(*currentBB_, pc);
        cache_.incCounter(pc);
    }
    hookManager_->triggerBBFetchHook(*currentBB_);
    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->currentInstr()].operation])();
}
void Hart::exec_INV_OP() { raiseIllegalInstruction(); }
void Hart::exec_ADDI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLTI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64DWord opnd2 = util::Signify(
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLTIU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_ORI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64DWord res = opnd1 | opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_ANDI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64DWord res = opnd1 & opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_XORI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLLI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRLI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRAI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>((*currentBB_)[instrIt].immidiate);
    RV64DWord res = opnd1 >> opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, util::Unsignify(res));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LUI() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 =
        util::ExtractBits<RV64UDWord, 20>((*currentBB_)[instrIt].immidiate);

    gprf_.write((*currentBB_)[instrIt].rd, opnd1 << 12);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_AUIPC() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord offset =
        util::ExtractBits<RV64UDWord, 20>((*currentBB_)[instrIt].immidiate);
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord res = pc + (offset << 12);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_ADD() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLT() {
    size_t instrIt = currentBB_->currentInstr();

    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLTU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_AND() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 & opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_OR() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 | opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_XOR() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLL() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRL() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SUB() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = opnd1 - opnd2;

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRA() {
    size_t instrIt = currentBB_->currentInstr();

    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_JAL() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 20>((*currentBB_)[instrIt].immidiate);

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write((*currentBB_)[instrIt].rd, ret);
    gprf_.write(exec::GPRF::PC, target);

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_JALR() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord base = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);

    RV64UDWord target = (base + offset) & ~(static_cast<RV64UDWord>(1));
    RV64UDWord ret = pc + 4;

    gprf_.write((*currentBB_)[instrIt].rd, ret);
    gprf_.write(exec::GPRF::PC, target);

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_BEQ() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);

    if (opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_BNE() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);

    if (opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_BLT() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs2));

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_BLTU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_BGE() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs2));

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_BGEU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}

void Hart::exec_LB() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64Char value = util::Signify(mmu_->loadByte(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write((*currentBB_)[instrIt].rd, extendedValue);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LH() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64HWord value = util::Signify(mmu_->loadHWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write((*currentBB_)[instrIt].rd, extendedValue);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64Word value = util::Signify(mmu_->loadWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write((*currentBB_)[instrIt].rd, extendedValue);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LD() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord value = mmu_->loadDWord(address);

    gprf_.write((*currentBB_)[instrIt].rd, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LBU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord value = mmu_->loadByte(address);

    gprf_.write((*currentBB_)[instrIt].rd, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LHU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord value = mmu_->loadHWord(address);

    gprf_.write((*currentBB_)[instrIt].rd, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_LWU() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord value = mmu_->loadWord(address);

    gprf_.write((*currentBB_)[instrIt].rd, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SB() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UChar value =
        static_cast<RV64UChar>(gprf_.read((*currentBB_)[instrIt].rs2));

    mmu_->storeByte(address, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SH() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UHWord value =
        static_cast<RV64UHWord>(gprf_.read((*currentBB_)[instrIt].rs2));

    mmu_->storeHWord(address, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UWord value =
        static_cast<RV64UWord>(gprf_.read((*currentBB_)[instrIt].rs2));

    mmu_->storeWord(address, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SD() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord address =
        gprf_.read((*currentBB_)[instrIt].rs1) +
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord value =
        static_cast<RV64UDWord>(gprf_.read((*currentBB_)[instrIt].rs2));

    mmu_->storeDWord(address, value);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}

// Does nothing in in-order implementation
void Hart::exec_FENCE() {
    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[currentBB_->currentInstr()]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_FENCE_TSO() {
    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[currentBB_->currentInstr()]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}

// todo: to be implemented
void Hart::exec_PAUSE() {
    this->raiseIllegalInstruction();

    hookManager_->triggerInstrExecHook((*currentBB_)[currentBB_->currentInstr()]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}

void Hart::exec_ECALL() {
    size_t instrIt = currentBB_->currentInstr();

    switch (csrf_.getPrivillege()) {
    case exec::PRIVILLEGE_USER:
        this->raiseException(EXCEPTION_ECALL_UMODE);
        break;

    case exec::PRIVILLEGE_SUPERVISOR:
        this->raiseException(EXCEPTION_ECALL_SMODE);
        break;

    case exec::PRIVILLEGE_MACHINE:
        this->raiseException(EXCEPTION_ECALL_MMODE);
        break;

    default:
        std::terminate();
        break;
    }

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_EBREAK() {
    size_t instrIt = currentBB_->currentInstr();

    this->raiseException(EXCEPTION_BREAKPOINT);

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}

void Hart::exec_ADDIW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLLIW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRLIW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRAIW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>((*currentBB_)[instrIt].immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_ADDW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SUBW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 - opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SLLW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRLW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64UDWord opnd1 = gprf_.read((*currentBB_)[instrIt].rs1);
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRAW() {
    size_t instrIt = currentBB_->currentInstr();

    RV64DWord opnd1 = util::Signify(gprf_.read((*currentBB_)[instrIt].rs1));
    RV64UDWord opnd2 = gprf_.read((*currentBB_)[instrIt].rs2);
    RV64UDWord res =
        util::ExtractBits<RV64UDWord, 32>(util::Unsignify(opnd1 >> opnd2));

    gprf_.write((*currentBB_)[instrIt].rd, res);

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_MRET() {
    size_t instrIt = currentBB_->currentInstr();

    if (csrf_.getPrivillege() != exec::PRIVILLEGE_MACHINE) {
        this->raiseIllegalInstruction();
    }

    csrf_.setPrivillege(csrf_.mstatus.get<exec::MStatus::MPP>());
    csrf_.mstatus.set<exec::MStatus::MIE>(
        csrf_.mstatus.get<exec::MStatus::MPIE>());
    csrf_.mstatus.set<exec::MStatus::MPIE>(1);
    csrf_.mstatus.set<exec::MStatus::MPP>(exec::PRIVILLEGE_USER);
    // csrf_.mstatus.set<exec::MStatus::MPRV>(0);

    gprf_.write(exec::GPRF::PC, csrf_.mepc.get<exec::MEPC::Value>());

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_SRET() {
    size_t instrIt = currentBB_->currentInstr();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    /*
    csrf_.setPrivillege(csrf_.mstatus.get<exec::MStatus::SPP>());
    csrf_.mstatus.set<exec::MStatus::SIE>(csrf_.mstatus.get<exec::MStatus::SPIE>());
    csrf_.mstatus.set<exec::MStatus::SPIE>(1);
    csrf_.mstatus.set<exec::MStatus::SPP>(PRIVILLEGE_USER);
    // csrf_.mstatus.set<exec::MStatus::MPRV>(0);

    gprf_.write(exec::GPRF::PC, csrf_.mepc.get<exec::MEPC::Value>());
    */
    std::terminate();
}
void Hart::exec_CSRRW() {
    size_t instrIt = currentBB_->currentInstr();

    auto status = csrf_.write((*currentBB_)[instrIt].immidiate,
                              gprf_.read((*currentBB_)[instrIt].rs1));
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write((*currentBB_)[instrIt].rd, std::get<RV64UDWord>(status));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_CSRRS() {
    size_t instrIt = currentBB_->currentInstr();

    std::variant<bool, RV64UDWord> status;
    if ((*currentBB_)[instrIt].rs1 == exec::GPRF::X0) {
        status = csrf_.read((*currentBB_)[instrIt].immidiate);
    } else {
        status = csrf_.setBits((*currentBB_)[instrIt].immidiate,
                               gprf_.read((*currentBB_)[instrIt].rs1));
    }

    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write((*currentBB_)[instrIt].rd, std::get<RV64UDWord>(status));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_CSRRC() {
    size_t instrIt = currentBB_->currentInstr();

    auto status = csrf_.clearBits((*currentBB_)[instrIt].immidiate,
                                  gprf_.read((*currentBB_)[instrIt].rs1));
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write((*currentBB_)[instrIt].rd, std::get<RV64UDWord>(status));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_CSRRWI() {
    size_t instrIt = currentBB_->currentInstr();

    auto status =
        csrf_.write((*currentBB_)[instrIt].immidiate, (*currentBB_)[instrIt].rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write((*currentBB_)[instrIt].rd, std::get<RV64UDWord>(status));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_CSRRSI() {
    size_t instrIt = currentBB_->currentInstr();

    auto status =
        csrf_.setBits((*currentBB_)[instrIt].immidiate, (*currentBB_)[instrIt].rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write((*currentBB_)[instrIt].rd, std::get<RV64UDWord>(status));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}
void Hart::exec_CSRRCI() {
    size_t instrIt = currentBB_->currentInstr();

    auto status =
        csrf_.clearBits((*currentBB_)[instrIt].immidiate, (*currentBB_)[instrIt].rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write((*currentBB_)[instrIt].rd, std::get<RV64UDWord>(status));

    this->nextPC();

    hookManager_->triggerInstrExecHook((*currentBB_)[instrIt]);

    (this->*HANDLER_ARR[(*currentBB_)[currentBB_->nextInstr()].operation])();
}

void Hart::nextPC() {
    gprf_.write(exec::GPRF::PC, gprf_.read(exec::GPRF::PC) + 4);
}

} // namespace besm::sim
