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

void Hart::run() { this->exec_BB_END(); }

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

    this->exec_BB_END();
}
void Hart::raiseIllegalInstruction() {
    this->raiseException(EXCEPTION_ILLEGAL_INSTR);
}

void Hart::assembleBB(exec::BasicBlock& bb, RV64Ptr pc) {
    exec::BasicBlockRebuilder rebuilder(bb, pc);

    while (rebuilder.append(dec_.parse(prefetcher_.loadWord(pc)))) {
        pc += IALIGN / 8;
    }
}

void Hart::execNextInstr() {
    ++instrsExecuted_;

    hookManager_->triggerInstrExecHook(*currentInstr_);

    ++currentInstr_;
    (this->*HANDLER_ARR[currentInstr_->operation])();
}

void Hart::exec_BB_END() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);

    auto [bbFound, bb] = bbCache_.lookup(pc);
    if (!bbFound) {
        this->assembleBB(bb, pc);
    }

    hookManager_->triggerBBFetchHook(bb);

    currentInstr_ = bb.getInstructions();

    (this->*HANDLER_ARR[currentInstr_->operation])();
}
void Hart::exec_INV_OP() { raiseIllegalInstruction(); }

void Hart::exec_ADDI() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}

void Hart::exec_SLTI() {
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}

void Hart::exec_SLTIU() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_ORI() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64DWord res = opnd1 | opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_ANDI() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64DWord res = opnd1 & opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_XORI() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SLLI() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>(currentInstr_->immidiate);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SRLI() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>(currentInstr_->immidiate);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SRAI() {
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64UDWord opnd2 =
        util::ExtractBits<RV64UDWord, 5>(currentInstr_->immidiate);
    RV64DWord res = opnd1 >> opnd2;

    gprf_.write(currentInstr_->rd, util::Unsignify(res));

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LUI() {
    RV64UDWord opnd1 =
        util::ExtractBits<RV64UDWord, 20>(currentInstr_->immidiate);

    gprf_.write(currentInstr_->rd, opnd1);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_AUIPC() {
    RV64UDWord offset =
        util::ExtractBits<RV64UDWord, 20>(currentInstr_->immidiate);
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord res = pc + (offset);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_ADD() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SLT() {
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(currentInstr_->rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SLTU() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_AND() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 & opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_OR() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 | opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_XOR() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SLL() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SRL() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SUB() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = opnd1 - opnd2;

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SRA() {
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_JAL() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 20>(currentInstr_->immidiate);

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write(currentInstr_->rd, ret);
    gprf_.write(exec::GPRF::PC, target);

    this->execNextInstr();
}
void Hart::exec_JALR() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord base = gprf_.read(currentInstr_->rs1);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);

    RV64UDWord target = (base + offset) & ~(static_cast<RV64UDWord>(1));
    RV64UDWord ret = pc + 4;

    gprf_.write(currentInstr_->rd, ret);
    gprf_.write(exec::GPRF::PC, target);

    this->execNextInstr();
}
void Hart::exec_BEQ() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);

    if (opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    this->execNextInstr();
}
void Hart::exec_BNE() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);

    if (opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    this->execNextInstr();
}
void Hart::exec_BLT() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(currentInstr_->rs2));

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    this->execNextInstr();
}
void Hart::exec_BLTU() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    this->execNextInstr();
}
void Hart::exec_BGE() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(currentInstr_->rs2));

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    this->execNextInstr();
}
void Hart::exec_BGEU() {
    RV64UDWord pc = gprf_.read(exec::GPRF::PC);
    RV64UDWord offset =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(exec::GPRF::PC, target);
    } else {
        this->nextPC();
    }

    this->execNextInstr();
}

void Hart::exec_LB() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64Char value = util::Signify(mmu_->loadByte(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(currentInstr_->rd, extendedValue);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LH() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64HWord value = util::Signify(mmu_->loadHWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(currentInstr_->rd, extendedValue);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LW() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64Word value = util::Signify(mmu_->loadWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(currentInstr_->rd, extendedValue);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LD() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord value = mmu_->loadDWord(address);

    gprf_.write(currentInstr_->rd, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LBU() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord value = mmu_->loadByte(address);

    gprf_.write(currentInstr_->rd, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LHU() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord value = mmu_->loadHWord(address);

    gprf_.write(currentInstr_->rd, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_LWU() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord value = mmu_->loadWord(address);

    gprf_.write(currentInstr_->rd, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SB() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UChar value = static_cast<RV64UChar>(gprf_.read(currentInstr_->rs2));

    mmu_->storeByte(address, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SH() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UHWord value = static_cast<RV64UHWord>(gprf_.read(currentInstr_->rs2));

    mmu_->storeHWord(address, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SW() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UWord value = static_cast<RV64UWord>(gprf_.read(currentInstr_->rs2));

    mmu_->storeWord(address, value);

    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_SD() {
    RV64UDWord address =
        gprf_.read(currentInstr_->rs1) +
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord value = static_cast<RV64UDWord>(gprf_.read(currentInstr_->rs2));

    mmu_->storeDWord(address, value);

    this->nextPC();

    this->execNextInstr();
}

// Does nothing in in-order implementation
void Hart::exec_FENCE() {
    this->nextPC();

    this->execNextInstr();
}
void Hart::exec_FENCE_TSO() {
    this->nextPC();

    this->execNextInstr();
}

// todo: to be implemented
void Hart::exec_PAUSE() {
    this->raiseIllegalInstruction();

    this->execNextInstr();
}

void Hart::exec_ECALL() {
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
    this->execNextInstr();
}
void Hart::exec_EBREAK() {
    return;

    this->raiseException(EXCEPTION_BREAKPOINT);
    this->execNextInstr();
}

void Hart::exec_ADDIW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SLLIW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SRLIW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SRAIW() {
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64UDWord opnd2 =
        util::SignExtend<RV64UDWord, 12>(currentInstr_->immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_ADDW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SUBW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 - opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SLLW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SRLW() {
    RV64UDWord opnd1 = gprf_.read(currentInstr_->rs1);
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_SRAW() {
    RV64DWord opnd1 = util::Signify(gprf_.read(currentInstr_->rs1));
    RV64UDWord opnd2 = gprf_.read(currentInstr_->rs2);
    RV64UDWord res =
        util::ExtractBits<RV64UDWord, 32>(util::Unsignify(opnd1 >> opnd2));

    gprf_.write(currentInstr_->rd, res);

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_MRET() {
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
    this->execNextInstr();
}
void Hart::exec_SRET() {
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
    auto status =
        csrf_.write(currentInstr_->immidiate, gprf_.read(currentInstr_->rs1));
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(currentInstr_->rd, std::get<RV64UDWord>(status));

    this->execNextInstr();
}
void Hart::exec_CSRRS() {
    std::variant<bool, RV64UDWord> status;
    if (currentInstr_->rs1 == exec::GPRF::X0) {
        status = csrf_.read(currentInstr_->immidiate);
    } else {
        status = csrf_.setBits(currentInstr_->immidiate,
                               gprf_.read(currentInstr_->rs1));
    }

    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(currentInstr_->rd, std::get<RV64UDWord>(status));

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_CSRRC() {
    auto status = csrf_.clearBits(currentInstr_->immidiate,
                                  gprf_.read(currentInstr_->rs1));
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(currentInstr_->rd, std::get<RV64UDWord>(status));

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_CSRRWI() {
    auto status = csrf_.write(currentInstr_->immidiate, currentInstr_->rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(currentInstr_->rd, std::get<RV64UDWord>(status));

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_CSRRSI() {
    auto status = csrf_.setBits(currentInstr_->immidiate, currentInstr_->rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(currentInstr_->rd, std::get<RV64UDWord>(status));

    this->nextPC();
    this->execNextInstr();
}
void Hart::exec_CSRRCI() {
    auto status = csrf_.clearBits(currentInstr_->immidiate, currentInstr_->rs1);
    if (std::holds_alternative<bool>(status)) {
        this->raiseIllegalInstruction();
        return;
    }

    gprf_.write(currentInstr_->rd, std::get<RV64UDWord>(status));

    this->nextPC();
    this->execNextInstr();
}

void Hart::nextPC() {
    gprf_.write(exec::GPRF::PC, gprf_.read(exec::GPRF::PC) + IALIGN / 8);
}

} // namespace besm::sim
