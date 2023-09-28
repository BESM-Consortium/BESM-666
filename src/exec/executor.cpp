#include "besm-666/exec/executor.hpp"
#include "besm-666/util/bit-magic.hpp"

#include <iostream>
namespace besm::exec {

void Executor::exec(Instruction const instr) {
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
    case PAUSE:
        // can't find this instruction in docs
        break;
    }
}

void Executor::exec_ADDI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = opnd1 + opnd2;
    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLTI(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 =
        util::Signify(util::SignExtend<RV64UDWord, 12>(instr.immidiate));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLTIU(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ORI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ANDI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_XORI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLLI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRLI(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRAI(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64DWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, util::Unsignify(res));

    this->nextPC();
}

void Executor::exec_LUI(Instruction const instr) {
    RV64UDWord opnd1 = util::ExtractBits<RV64UDWord, 20>(instr.immidiate);

    gprf_.write(instr.rd, opnd1 << 12);

    this->nextPC();
}

void Executor::exec_AUIPC(Instruction const instr) {
    RV64UDWord offset = util::ExtractBits<RV64UDWord, 20>(instr.immidiate);
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord res = pc + (offset << 12);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ADD(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLT(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLTU(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_AND(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_OR(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_XOR(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLL(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRL(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SUB(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 - opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRA(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_JAL(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 20>(instr.immidiate) << 1;

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(GPRF::PC, target);
}

void Executor::exec_JALR(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord base = gprf_.read(instr.rs1);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;

    RV64UDWord target = (base + offset) & ~(static_cast<RV64UDWord>(1));
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(GPRF::PC, target);
}

void Executor::exec_BEQ(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BNE(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BLT(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BLTU(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BGE(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BGEU(Instruction const instr) {
    RV64UDWord pc = gprf_.read(GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_LB(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64Char value = util::Signify(mmu_->loadByte(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(instr.rd, extendedValue);

    this->nextPC();
}
void Executor::exec_LH(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64HWord value = util::Signify(mmu_->loadHWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(instr.rd, extendedValue);

    this->nextPC();
}
void Executor::exec_LW(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64Word value = util::Signify(mmu_->loadWord(address));
    RV64UDWord extendedValue = util::Unsignify(static_cast<RV64DWord>(value));

    gprf_.write(instr.rd, extendedValue);

    this->nextPC();
}
void Executor::exec_LD(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadDWord(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Executor::exec_LBU(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadByte(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Executor::exec_LHU(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadHWord(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Executor::exec_LWU(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = mmu_->loadWord(address);

    gprf_.write(instr.rd, value);

    this->nextPC();
}
void Executor::exec_SB(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UChar value = static_cast<RV64UChar>(gprf_.read(instr.rs2));

    mmu_->storeByte(address, value);

    this->nextPC();
}
void Executor::exec_SH(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UHWord value = static_cast<RV64UHWord>(gprf_.read(instr.rs2));

    mmu_->storeHWord(address, value);

    this->nextPC();
}
void Executor::exec_SW(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UWord value = static_cast<RV64UWord>(gprf_.read(instr.rs2));

    mmu_->storeWord(address, value);

    this->nextPC();
}
void Executor::exec_SD(Instruction const instr) {
    RV64UDWord address = gprf_.read(instr.rs1) +
                         util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord value = static_cast<RV64UDWord>(gprf_.read(instr.rs2));

    mmu_->storeDWord(address, value);

    this->nextPC();
}

void Executor::exec_ADDIW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLLIW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRLIW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRAIW(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ADDW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SUBW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 - opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLLW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRLW(Instruction const instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRAW(Instruction const instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res =
        util::ExtractBits<RV64UDWord, 32>(util::Unsignify(opnd1 >> opnd2));

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::nextPC() { gprf_.write(GPRF::PC, gprf_.read(GPRF::PC) + 4); }

} // namespace besm::exec
