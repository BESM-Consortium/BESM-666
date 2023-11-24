#include <cassert>
#include <iostream>
#include <utility>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"

namespace besm::sim {

Hart::SPtr Hart::Create(std::shared_ptr<mem::PhysMem> const &pMem,
                        std::shared_ptr<HookManager> const &hookManager) {
    return std::shared_ptr<Hart>(new Hart(pMem, hookManager));
}

Hart::Hart(std::shared_ptr<mem::PhysMem> const &pMem,
           std::shared_ptr<HookManager> hookManager)
    : mmu_(mem::MMU::Create(pMem)), prefetcher_(mmu_),
      hookManager_(std::move(hookManager)), instrsExecuted_(0),
      currentInstr_(nullptr) {
    assert(mmu_ != nullptr);
}

bool Hart::finished() const { return false; }

void Hart::run() { exec_BB_END(*this); }

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

    this->fetchBB();
}
void Hart::raiseIllegalInstruction() {
    raiseException(EXCEPTION_ILLEGAL_INSTR);
}

void Hart::assembleBB(exec::BasicBlock &bb, RV64Ptr pc) {
    exec::BasicBlockRebuilder rebuilder(bb, pc);

    while (rebuilder.append(dec_.parse(prefetcher_.loadWord(pc)))) {
        pc += IALIGN / 8;
    }
}

void Hart::fetchBB() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);

    auto [bbFound, bb] = bbCache_.lookup(pc);
    if (!bbFound) {
        this->assembleBB(bb, pc);
    }

    hookManager_->triggerBBFetchHook(bb);

    currentInstr_ = bb.getInstructions();
}

inline void Hart::execNextInstr(Hart &hart) {
    ++hart.instrsExecuted_;

    hart.hookManager_->triggerInstrExecHook(*hart.currentInstr_);

    ++hart.currentInstr_;
    (*HANDLER_ARR[hart.currentInstr_->operation])(hart);
}

void Hart::exec_BB_END(Hart &hart) {
    hart.fetchBB();

    (*HANDLER_ARR[hart.currentInstr_->operation])(hart);
}
void Hart::exec_INV_OP(Hart &hart) { hart.raiseIllegalInstruction(); }

void Hart::exec_ADDI(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord res = opnd1 + opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}

void Hart::exec_SLTI(Hart &hart) {
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}

void Hart::exec_SLTIU(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_ORI(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64DWord res = opnd1 | opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_ANDI(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64DWord res = opnd1 & opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_XORI(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SLLI(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>(hart.currentInstr_->immidiate);
    RV64UDWord res = opnd1 << opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SRLI(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>(hart.currentInstr_->immidiate);
    RV64UDWord res = opnd1 >> opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SRAI(Hart &hart) {
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>(hart.currentInstr_->immidiate);
    RV64DWord res = opnd1 >> opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, util::Unsignify(res));

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LUI(Hart &hart) {
    RV64UDWord opnd1 =
        util::ExtractBits<RV64UDWord, 20>(hart.currentInstr_->immidiate);

    hart.gprf_.write(hart.currentInstr_->rd, opnd1);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_AUIPC(Hart &hart) {
    RV64UDWord offset =
        util::ExtractBits<RV64UDWord, 20>(hart.currentInstr_->immidiate);
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord res = pc + (offset);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_ADD(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 + opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SLT(Hart &hart) {
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SLTU(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_AND(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 & opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_OR(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 | opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_XOR(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SLL(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 << opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SRL(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 >> opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SUB(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = opnd1 - opnd2;

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SRA(Hart &hart) {
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_JAL(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 20>(hart.currentInstr_->immidiate);

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    hart.gprf_.write(hart.currentInstr_->rd, ret);
    hart.gprf_.write(exec::GPRF::PC, target);

    execNextInstr(hart);
}
void Hart::exec_JALR(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord base = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);

    RV64UDWord target = (base + offset) & ~(static_cast<RV64UDWord>(1));
    RV64UDWord ret = pc + 4;

    hart.gprf_.write(hart.currentInstr_->rd, ret);
    hart.gprf_.write(exec::GPRF::PC, target);

    execNextInstr(hart);
}
void Hart::exec_BEQ(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);

    if (opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        hart.gprf_.write(exec::GPRF::PC, target);
    } else {
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_BNE(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);

    if (opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        hart.gprf_.write(exec::GPRF::PC, target);
    } else {
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_BLT(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs2));

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        hart.gprf_.write(exec::GPRF::PC, target);
    } else {
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_BLTU(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        hart.gprf_.write(exec::GPRF::PC, target);
    } else {
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_BGE(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs2));

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        hart.gprf_.write(exec::GPRF::PC, target);
    } else {
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_BGEU(Hart &hart) {
    RV64UDWord pc = hart.gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        hart.gprf_.write(exec::GPRF::PC, target);
    } else {
        hart.nextPC();
    }

    execNextInstr(hart);
}

void Hart::exec_LB(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64Char value = util::Signify(hart.mmu_->loadByte(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    hart.gprf_.write(hart.currentInstr_->rd, extendedValue);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LH(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64HWord value = util::Signify(hart.mmu_->loadHWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    hart.gprf_.write(hart.currentInstr_->rd, extendedValue);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LW(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64Word value = util::Signify(hart.mmu_->loadWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    hart.gprf_.write(hart.currentInstr_->rd, extendedValue);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LD(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord value = hart.mmu_->loadDWord(address);

    hart.gprf_.write(hart.currentInstr_->rd, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LBU(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord value = hart.mmu_->loadByte(address);

    hart.gprf_.write(hart.currentInstr_->rd, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LHU(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord value = hart.mmu_->loadHWord(address);

    hart.gprf_.write(hart.currentInstr_->rd, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_LWU(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord value = hart.mmu_->loadWord(address);

    hart.gprf_.write(hart.currentInstr_->rd, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SB(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UChar value =
        static_cast<RV64UChar>(hart.gprf_.read(hart.currentInstr_->rs2));

    hart.mmu_->storeByte(address, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SH(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UHWord value =
        static_cast<RV64UHWord>(hart.gprf_.read(hart.currentInstr_->rs2));

    hart.mmu_->storeHWord(address, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SW(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UWord value =
        static_cast<RV64UWord>(hart.gprf_.read(hart.currentInstr_->rs2));

    hart.mmu_->storeWord(address, value);

    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_SD(Hart &hart) {
    RV64UDWord address =
        hart.gprf_.read(hart.currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord value =
        static_cast<RV64UDWord>(hart.gprf_.read(hart.currentInstr_->rs2));

    hart.mmu_->storeDWord(address, value);

    hart.nextPC();

    execNextInstr(hart);
}

// Does nothing in in-order implementation
void Hart::exec_FENCE(Hart &hart) {
    hart.nextPC();

    execNextInstr(hart);
}
void Hart::exec_FENCE_TSO(Hart &hart) {
    hart.nextPC();

    execNextInstr(hart);
}

// todo: to be implemented
void Hart::exec_PAUSE(Hart &hart) {
    hart.raiseIllegalInstruction();

    execNextInstr(hart);
}

void Hart::exec_ECALL(Hart &hart) {
    switch (hart.csrf_.getPrivillege()) {
    case exec::PRIVILLEGE_USER:
        hart.raiseException(EXCEPTION_ECALL_UMODE);
        break;

    case exec::PRIVILLEGE_SUPERVISOR:
        hart.raiseException(EXCEPTION_ECALL_SMODE);
        break;

    case exec::PRIVILLEGE_MACHINE:
        hart.raiseException(EXCEPTION_ECALL_MMODE);
        break;

    default:
        std::terminate();
        break;
    }
    execNextInstr(hart);
}
void Hart::exec_EBREAK(Hart &hart) {
    return;

    hart.raiseException(EXCEPTION_BREAKPOINT);
    execNextInstr(hart);
}

void Hart::exec_ADDIW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SLLIW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SRLIW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SRAIW(Hart &hart) {
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(hart.currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_ADDW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SUBW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 - opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SLLW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SRLW(Hart &hart) {
    RV64UDWord opnd1 = hart.gprf_.read(hart.currentInstr_->rs1);
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_SRAW(Hart &hart) {
    RV64DWord opnd1 = util::Signify(hart.gprf_.read(hart.currentInstr_->rs1));
    RV64UDWord opnd2 = hart.gprf_.read(hart.currentInstr_->rs2);
    RV64UDWord res =
        util::ExtractBits<RV64UDWord, 32>(util::Unsignify(opnd1 >> opnd2));

    hart.gprf_.write(hart.currentInstr_->rd, res);

    hart.nextPC();
    execNextInstr(hart);
}
void Hart::exec_MRET(Hart &hart) {
    if (hart.csrf_.getPrivillege() != exec::PRIVILLEGE_MACHINE) {
        hart.raiseIllegalInstruction();
    } else {
        hart.csrf_.setPrivillege(hart.csrf_.mstatus.get<exec::MStatus::MPP>());
        hart.csrf_.mstatus.set<exec::MStatus::MIE>(
            hart.csrf_.mstatus.get<exec::MStatus::MPIE>());
        hart.csrf_.mstatus.set<exec::MStatus::MPIE>(1);
        hart.csrf_.mstatus.set<exec::MStatus::MPP>(exec::PRIVILLEGE_USER);
        // hart.csrf_.mstatus.set<exec::MStatus::MPRV>(0);
    }

    hart.gprf_.write(exec::GPRF::PC, hart.csrf_.mepc.get<exec::MEPC::Value>());
    execNextInstr(hart);
}
void Hart::exec_SRET(Hart &hart) {
    /*
    hart.csrf_.setPrivillege(hart.csrf_.mstatus.get<exec::MStatus::SPP>());
    hart.csrf_.mstatus.set<exec::MStatus::SIE>(hart.csrf_.mstatus.get<exec::MStatus::SPIE>());
    hart.csrf_.mstatus.set<exec::MStatus::SPIE>(1);
    hart.csrf_.mstatus.set<exec::MStatus::SPP>(PRIVILLEGE_USER);
    // hart.csrf_.mstatus.set<exec::MStatus::MPRV>(0);

    hart.gprf_.write(exec::GPRF::PC, hart.csrf_.mepc.get<exec::MEPC::Value>());
    */
    std::terminate();
}
void Hart::exec_CSRRW(Hart &hart) {
    auto status = hart.csrf_.write(hart.currentInstr_->immidiate,
                                   hart.gprf_.read(hart.currentInstr_->rs1));
    if (std::holds_alternative<bool>(status)) {
        hart.raiseIllegalInstruction();
    } else {
        hart.gprf_.write(hart.currentInstr_->rd, std::get<RV64UDWord>(status));
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_CSRRS(Hart &hart) {
    std::variant<bool, RV64UDWord> status;
    if (hart.currentInstr_->rs1 == exec::GPRF::X0) {
        status = hart.csrf_.read(hart.currentInstr_->immidiate);
    } else {
        status = hart.csrf_.setBits(hart.currentInstr_->immidiate,
                                    hart.gprf_.read(hart.currentInstr_->rs1));
    }

    if (std::holds_alternative<bool>(status)) {
        hart.raiseIllegalInstruction();
    } else {
        hart.gprf_.write(hart.currentInstr_->rd, std::get<RV64UDWord>(status));
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_CSRRC(Hart &hart) {
    auto status =
        hart.csrf_.clearBits(hart.currentInstr_->immidiate,
                             hart.gprf_.read(hart.currentInstr_->rs1));
    if (std::holds_alternative<bool>(status)) {
        hart.raiseIllegalInstruction();
    } else {
        hart.gprf_.write(hart.currentInstr_->rd, std::get<RV64UDWord>(status));
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_CSRRWI(Hart &hart) {
    auto status = hart.csrf_.write(hart.currentInstr_->immidiate,
                                   hart.currentInstr_->rs1);
    if (std::holds_alternative<bool>(status)) {
        hart.raiseIllegalInstruction();
    } else {
        hart.gprf_.write(hart.currentInstr_->rd, std::get<RV64UDWord>(status));
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_CSRRSI(Hart &hart) {
    auto status = hart.csrf_.setBits(hart.currentInstr_->immidiate,
                                     hart.currentInstr_->rs1);
    if (std::holds_alternative<bool>(status)) {
        hart.raiseIllegalInstruction();
    } else {
        hart.gprf_.write(hart.currentInstr_->rd, std::get<RV64UDWord>(status));
        hart.nextPC();
    }

    execNextInstr(hart);
}
void Hart::exec_CSRRCI(Hart &hart) {
    auto status = hart.csrf_.clearBits(hart.currentInstr_->immidiate,
                                       hart.currentInstr_->rs1);
    if (std::holds_alternative<bool>(status)) {
        hart.raiseIllegalInstruction();
    } else {
        hart.gprf_.write(hart.currentInstr_->rd, std::get<RV64UDWord>(status));
        hart.nextPC();
    }

    execNextInstr(hart);
}

void Hart::nextPC() {
    gprf_.write(exec::GPRF::PC, gprf_.read(exec::GPRF::PC) + IALIGN / 8);
}

} // namespace besm::sim
