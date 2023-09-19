#include "besm-666/exec/executor.hpp"
#include "besm-666/util/bit-magic.hpp"

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
    case FENCE:
        exec_FENCE(instr);
        break;
    }
}

void Executor::exec_ADDI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<Immidiate, 12>(instr.immidiate);
    RV64UDWord res = opnd1 + opnd2;
    gprf_.write(instr.rd, res);
}

void Executor::exec_SLTI(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 =
        util::Signify(util::SignExtend<Immidiate, 12>(instr.immidiate));
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SLTIU(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<Immidiate, 12>(instr.immidiate);
    RV64DWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);
}

void Executor::exec_ORI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<Immidiate, 12>(instr.immidiate);
    RV64DWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_ANDI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<Immidiate, 12>(instr.immidiate);
    RV64DWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_XORI(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = util::SignExtend<Immidiate, 12>(instr.immidiate);
    RV64DWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_LUI(Instruction instr) {
    RV64UDWord opnd1 = util::ExtractBits<Immidiate, 20>(instr.immidiate);

    gprf_.write(instr.rd, opnd1 << 12);
}

void Executor::exec_AUIPC(Instruction instr) {
    RV64UDWord offset = util::ExtractBits<Immidiate, 20>(instr.immidiate);
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord res = pc + offset;

    gprf_.write(instr.rd, res);
}

void Executor::exec_ADD(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 + opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SLT(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SLTU(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 < opnd2 ? 1 : 0;

    gprf_.write(instr.rd, res);
}

void Executor::exec_AND(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 & opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_OR(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 | opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_XOR(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 ^ opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SLL(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 << opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SLR(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 >> opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SUB(Instruction instr) {
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = opnd1 - opnd2;

    gprf_.write(instr.rd, res);
}

void Executor::exec_SRA(Instruction instr) {
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64UDWord opnd2 = gprf_.read(instr.rs2);
    RV64UDWord res = util::Unsignify(opnd1 >> opnd2);

    gprf_.write(instr.rd, res);
}

void Executor::exec_JAL(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 20>(instr.immidiate) << 1;

    RV64UDWord target = pc + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(reg::GPRF::PC, target);
}

void Executor::exec_JALR(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord base = gprf_.read(instr.rs1);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;

    RV64UDWord target = base + offset;
    RV64UDWord ret = pc + 4;

    gprf_.write(instr.rd, ret);
    gprf_.write(reg::GPRF::PC, target);
}

void Executor::exec_BEQ(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if(opnd1 == opnd2) {
        RV64UDWord target = pc + offset;
        RV64UDWord ret = pc + 4;

        gprf_.write(instr.rd, ret);
        gprf_.write(reg::GPRF::PC, target);
    }
}

void Executor::exec_BNE(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if(opnd1 != opnd2) {
        RV64UDWord target = pc + offset;
        RV64UDWord ret = pc + 4;

        gprf_.write(instr.rd, ret);
        gprf_.write(reg::GPRF::PC, target);
    }
}

void Executor::exec_BLT(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if(opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        RV64UDWord ret = pc + 4;

        gprf_.write(instr.rd, ret);
        gprf_.write(reg::GPRF::PC, target);
    }
}

void Executor::exec_BLTU(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if(opnd1 < opnd2) {
        RV64UDWord target = pc + offset;
        RV64UDWord ret = pc + 4;

        gprf_.write(instr.rd, ret);
        gprf_.write(reg::GPRF::PC, target);
    }
}

void Executor::exec_BGE(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;
    RV64DWord opnd1 = util::Signify(gprf_.read(instr.rs1));
    RV64DWord opnd2 = util::Signify(gprf_.read(instr.rs2));

    if(opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        RV64UDWord ret = pc + 4;

        gprf_.write(instr.rd, ret);
        gprf_.write(reg::GPRF::PC, target);
    }
}

void Executor::exec_BGEU(Instruction instr) {
    RV64UDWord pc = gprf_.read(reg::GPRF::PC);
    RV64UDWord offset = util::SignExtend<Immidiate, 12>(instr.immidiate) << 1;
    RV64UDWord opnd1 = gprf_.read(instr.rs1);
    RV64UDWord opnd2 = gprf_.read(instr.rs2);

    if(opnd1 >= opnd2) {
        RV64UDWord target = pc + offset;
        RV64UDWord ret = pc + 4;

        gprf_.write(instr.rd, ret);
        gprf_.write(reg::GPRF::PC, target);
    }
}


} // namespace besm::exec
