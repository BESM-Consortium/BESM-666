#include "besm-666/exec/executor.hpp"
#include "besm-666/util/bit-magic.hpp"

namespace besm::exec {

void Executor::exec(Instruction instr) {
    switch (instr.operation) {
    case ADDI:
        exec_ADDI(instr);
        break;
    case SLTI:
        exec_SLTI(instr);
        break;
    case SLTU:
        exec_SLTU(instr);
        break;
    case ORI:
        exec_ORI(instr);
        break;
    case ANDI:
        exec_ANDI(instr);
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

void Executor::exec_SLTU(Instruction instr) {
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

} // namespace besm::exec
