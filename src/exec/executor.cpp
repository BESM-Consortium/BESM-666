#include "besm-666/exec/executor.hpp"
#include "besm-666/util/bit-magic.hpp"

#include <iostream>
namespace besm::exec {

void Executor::exec(Instruction instr) {
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

void Executor::exec_ADDI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = opnd1 + opnd2;
    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLTI(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 =
        util::Signify(util::SignExtend<RV64UDWord, 12>(instr.immidiate));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLTIU(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ORI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ANDI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_XORI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLLI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRLI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRAI(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = util::ExtractBits<RV64UDWord, 5>(instr.immidiate);
    RV64DWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, util::Unsignify(res));

    this->nextPC();
}

void Executor::exec_LUI(Instruction instr) {
    RV64UDWord opnd1 = util::ExtractBits<RV64UDWord, 20>(instr.immidiate);

    gprf_.write(instr.rd, opnd1 << 12);

    this->nextPC();
}

void Executor::exec_AUIPC(Instruction instr) {
    RV64UDWord offset = util::ExtractBits<RV64UDWord, 20>(instr.immidiate);
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord res = pc + (offset << 12);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ADD(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLT(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLTU(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_AND(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_OR(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_XOR(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLL(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRL(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SUB(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 - opnd2;

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRA(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_JAL(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 20>(instr.immidiate) << 1;

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(reg::GPRF::PC, target);
}

void Executor::exec_JALR(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord base = gprf_.read(instr.rs1);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;

    RV64UDWord target = (base + offset) & ~(static_cast<RV64UDWord>(1));
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(reg::GPRF::PC, target);
}

void Executor::exec_BEQ(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(reg::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BNE(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(reg::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BLT(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(reg::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BLTU(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(reg::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BGE(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(reg::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_BGEU(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<RV64UDWord, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if (opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        gprf_.write(reg::GPRF::PC, target);
    } else {
        this->nextPC();
    }
}

void Executor::exec_ADDIW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLLIW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRLIW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRAIW(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = util::SignExtend<RV64UDWord, 12>(instr.immidiate);
    RV64UDWord res = util::Unsignify(util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2));

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_ADDW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 + opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SUBW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 - opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SLLW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 << opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRLW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::exec_SRAW(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::Unsignify(util::ExtractBits<RV64UDWord, 32>(opnd1 >> opnd2));

    gprf_.write(instr.rd, res);

    this->nextPC();
}

void Executor::nextPC() {
    gprf_.write(reg::GPRF::PC, gprf_.read(reg::GPRF::PC) + 4);
}

} // namespace besm::exec
