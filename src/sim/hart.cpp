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

void Hart::exec(const Instruction instr) {
    switch (instr.operation) {
    // RV32I
    case ADDI:
        exec_ADDI(instr);
        break;
    case SLTI:
        exec_SLTI(instr);
        break;
    case SLTIU:
        exec_SLTIU(instr);
        break;
    case ANDI:
        exec_ANDI(instr);
        break;
    case ORI:
        exec_ORI(instr);
        break;
    case XORI:
        exec_XORI(instr);
        break;
    case SLLI:
        exec_SLLI(instr);
        break;
    case SRLI:
        exec_SRLI(instr);
        break;
    case SRAI:
        exec_SRAI(instr);
        break;
    case LUI:
        exec_LUI(instr);
        break;
    case AUIPC:
        exec_AUIPC(instr);
        break;
    case ADD:
        exec_ADD(instr);
        break;
    case SLT:
        exec_SLT(instr);
        break;
    case SLTU:
        exec_SLTU(instr);
        break;
    case AND:
        exec_AND(instr);
        break;
    case OR:
        exec_OR(instr);
        break;
    case XOR:
        exec_XOR(instr);
        break;
    case SLL:
        exec_SLL(instr);
        break;
    case SRL:
        exec_SRL(instr);
        break;
    case SUB:
        exec_SUB(instr);
        break;
    case SRA:
        exec_SRA(instr);
        break;
    case JAL:
        exec_JAL(instr);
        break;
    case JALR:
        exec_JALR(instr);
        break;
    case BEQ:
        exec_BEQ(instr);
        break;
    case BNE:
        exec_BNE(instr);
        break;
    case BLT:
        exec_BLT(instr);
        break;
    case BLTU:
        exec_BLTU(instr);
        break;
    case BGE:
        exec_BGE(instr);
        break;
    case BGEU:
        exec_BGEU(instr);
        break;
    case LB:
        exec_LB(instr);
        break;
    case LH:
        exec_LH(instr);
        break;
    case LW:
        exec_LW(instr);
        break;
    case LD:
        exec_LD(instr);
        break;
    case LBU:
        exec_LBU(instr);
        break;
    case LHU:
        exec_LHU(instr);
        break;
    case LWU:
        exec_LWU(instr);
        break;
    case SB:
        exec_SB(instr);
        break;
    case SH:
        exec_SH(instr);
        break;
    case SW:
        exec_SW(instr);
        break;
    case SD:
        exec_SW(instr);
        break;
    case FENCE:
        exec_FENCE(instr);
        break;
    case FENCE_TSO:
        exec_FENCE_TSO(instr);
        break;
    case ECALL:
        exec_ECALL(instr);
        break;
    case EBREAK:
        exec_EBREAK(instr);
        break;
    case ADDIW:
        exec_ADDIW(instr);
        break;
    case SLLIW:
        exec_SLLIW(instr);
        break;
    case SRLIW:
        exec_SRLIW(instr);
        break;
    case SRAIW:
        exec_SRAIW(instr);
        break;
    case ADDW:
        exec_ADDW(instr);
        break;
    case SUBW:
        exec_SUBW(instr);
        break;
    case SLLW:
        exec_SLLW(instr);
        break;
    case SRLW:
        exec_SRLW(instr);
        break;
    case SRAW:
        exec_SRAW(instr);
        break;
    case MRET:
        exec_MRET(instr);
        break;
    case SRET:
        exec_SRET(instr);
        break;
    case CSRRW:
        exec_CSRRW(instr);
        break;
    case CSRRS:
        exec_CSRRS(instr);
        break;
    case CSRRC:
        exec_CSRRC(instr);
        break;
    case CSRRWI:
        exec_CSRRWI(instr);
        break;
    case CSRRSI:
        exec_CSRRSI(instr);
        break;
    case CSRRCI:
        exec_CSRRCI(instr);
        break;
    default:
        this->raiseIllegalInstruction();
        break;
    }
}
void Hart::execBB(const BasicBlock &bb) {
    auto it = bb.nextInstr();
    while (it != bb.end()) {
        exec(*it);
        hookManager_->triggerInstrExecHook(*it);
        if (exceptionHappened_) {
            exceptionHappened_ = false;
            break;
        }

        it = bb.nextInstr();
    }
}

Hart::Hart(std::shared_ptr<mem::PhysMem> const &pMem,
           std::shared_ptr<HookManager> hookManager)
    : mmu_(mem::MMU::Create(pMem)), dec_(mmu_),
      hookManager_(std::move(hookManager)),
      prevPC_(std::numeric_limits<RV64UDWord>::max()), instrsExecuted_(0),
      cache_(4, 128) {}

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
bool Hart::finished() const { return gprf_.read(exec::GPRF::PC) == prevPC_; }

void Hart::run() {
    static_assert(sizeof(sim::Hart::HANDLER_ARR) / sizeof(sim::Hart::Handler) ==
                  (InstructionOp::MRET + 1));
    while (!this->finished()) {
        this->runCycle();
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

void Hart::exec_INV_OP(Instruction const instr) { raiseIllegalInstruction(); }
void Hart::exec_ADDI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = opnd1 + opnd2;
    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLTI(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 =
        util::Signify(util::SignExtend<RV64UDWord, 12>(instr.immidiate));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLTIU(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_ORI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_ANDI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_XORI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLLI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRLI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRAI(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64DWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, util::Unsignify(res));

    this->nextPC();
}
void Hart::exec_LUI(Instruction const instr) {
    RV64UDWord opnd1 = util::ExtractBits<RV64UDWord, 20>(instr.immidiate);

    gprf_.write(instr.rd, opnd1 << 12);

    this->nextPC();
}
void Hart::exec_AUIPC(Instruction const instr) {
    RV64UDWord offset = util::ExtractBits<RV64UDWord, 20>(instr.immidiate);
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord res = pc + (offset << 12);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_ADD(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLT(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLTU(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_AND(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_OR(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_XOR(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLL(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRL(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SUB(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 - opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRA(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_JAL(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 20>(instr.immidiate);

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(exec::GPRF::PC, target);
}
void Hart::exec_JALR(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord base = gprf_.read(instr.rs1);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);

    RV64UDWord target = (base + offset) & ~(static_cast<RV64UDWord>(1));
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(exec::GPRF::PC, target);
}
void Hart::exec_BEQ(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}
void Hart::exec_BNE(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}
void Hart::exec_BLT(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}
void Hart::exec_BLTU(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}
void Hart::exec_BGE(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}
void Hart::exec_BGEU(Instruction const instr) {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Hart::exec_LB(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64Char value = util::Signify(mmu_->loadByte(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(instr.rd, extendedValue);

    this->nextPC();
}
void Hart::exec_LH(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64HWord value = util::Signify(mmu_->loadHWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(instr.rd, extendedValue);

    this->nextPC();
}
void Hart::exec_LW(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64Word value = util::Signify(mmu_->loadWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(instr.rd, extendedValue);

    this->nextPC();
}
void Hart::exec_LD(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadDWord(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Hart::exec_LBU(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadByte(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Hart::exec_LHU(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadHWord(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Hart::exec_LWU(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadWord(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Hart::exec_SB(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UChar value = static_cast<RV64UChar>(gprf_.read(instr.rs2));

    mmu_->storeByte(address, value);

    this->nextPC();
}
void Hart::exec_SH(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UHWord value = static_cast<RV64UHWord>(gprf_.read(instr.rs2));

    mmu_->storeHWord(address, value);

    this->nextPC();
}
void Hart::exec_SW(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UWord value = static_cast<RV64UWord>(gprf_.read(instr.rs2));

    mmu_->storeWord(address, value);

    this->nextPC();
}
void Hart::exec_SD(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = static_cast<RV64UDWord>(gprf_.read(instr.rs2));

    mmu_->storeDWord(address, value);

    this->nextPC();
}

void Hart::exec_ECALL(Instruction const instr) {
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
}
void Hart::exec_EBREAK(Instruction const instr) {
    this->raiseException(EXCEPTION_BREAKPOINT);
}

void Hart::exec_ADDIW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLLIW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRLIW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRAIW(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_ADDW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SUBW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 - opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SLLW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRLW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_SRAW(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res =
        util::ExtractBits<RV64UDWord, 32>(util::Unsignify(opnd1 >> opnd2));

    gprf_.write(instr.rd, res);

    this->nextPC();
}
void Hart::exec_MRET(Instruction const instr) {
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
}
void Hart::exec_SRET(Instruction const instr) {
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
void Hart::exec_CSRRW(Instruction const instr) {
    auto status = csrf_.write(instr.immidiate, gprf_.read(instr.rs1));
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(instr.rd, std::get<RV64UDWord>(status));

    this->nextPC();
}
void Hart::exec_CSRRS(Instruction const instr) {
    std::variant<bool, RV64UDWord> status;
    if (instr.rs1 == exec::GPRF::X0) {
        status = csrf_.read(instr.immidiate);
    } else {
        status = csrf_.setBits(instr.immidiate, gprf_.read(instr.rs1));
    }

    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(instr.rd, std::get<RV64UDWord>(status));

    this->nextPC();
}
void Hart::exec_CSRRC(Instruction const instr) {
    auto status = csrf_.clearBits(instr.immidiate, gprf_.read(instr.rs1));
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(instr.rd, std::get<RV64UDWord>(status));

    this->nextPC();
}
void Hart::exec_CSRRWI(Instruction const instr) {
    auto status = csrf_.write(instr.immidiate, instr.rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(instr.rd, std::get<RV64UDWord>(status));

    this->nextPC();
}
void Hart::exec_CSRRSI(Instruction const instr) {
    auto status = csrf_.setBits(instr.immidiate, instr.rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(instr.rd, std::get<RV64UDWord>(status));

    this->nextPC();
}
void Hart::exec_CSRRCI(Instruction const instr) {
    auto status = csrf_.clearBits(instr.immidiate, instr.rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(instr.rd, std::get<RV64UDWord>(status));

    this->nextPC();
}

void Hart::nextPC() {
    gprf_.write(exec::GPRF::PC, gprf_.read(exec::GPRF::PC) + 4);
}

} // namespace besm::sim
