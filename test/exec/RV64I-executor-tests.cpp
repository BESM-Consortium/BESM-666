#include <gtest/gtest.h>

#include "besm-666/exec/executor.hpp"
#include "besm-666/reg/rf.hpp"
#include "besm-666/util/bit-magic.hpp"

using namespace besm;

class RV64IExecutorTest : public testing::Test {
protected:
    void SetupInstrR(InstructionOp op, Register rd, Register rs1,
                     Register rs2) {
        instr.operation = op;
        instr.rd = rd;
        instr.rs1 = rs1;
        instr.rs2 = rs2;
    }
    void SetupInstrI(InstructionOp op, Register rd, Register rs1,
                     RV64UDWord imm) {
        instr.operation = op;
        instr.rd = rd;
        instr.rs1 = rs1;
        instr.immidiate = imm;
    }
    void SetupInstrIS(InstructionOp op, Register rd, Register rs1,
                      RV64DWord imm) {
        SetupInstrI(op, rd, rs1, util::Unsignify(imm));
    }
    void SetupInstrU(InstructionOp op, Register rd, RV64UDWord imm) {
        instr.operation = op;
        instr.rd = rd;
        instr.immidiate = imm;
    }
    void SetupInstrB(InstructionOp op, Register rs1, Register rs2,
                     RV64UDWord imm) {
        instr.operation = op;
        instr.rs1 = rs1;
        instr.rs2 = rs2;
        instr.immidiate = imm;
    }

    void LoadImm12(Register rd, RV64DWord val) {
        SetupInstrIS(InstructionOp::ADDI, rd, reg::GPRF::X0, val);
        Exec();
    }

    RV64UDWord ReadReg(Register r) { return exec.getState().read(r); }
    RV64DWord ReadRegS(Register r) {
        return util::Signify(exec.getState().read(r));
    }

    void Exec() { exec.exec(instr); }

    Instruction instr;
    exec::Executor exec;
};

TEST_F(RV64IExecutorTest, MOVI) {
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    LoadImm12(reg::GPRF::X1, -3);
    LoadImm12(reg::GPRF::X2, 3);

    EXPECT_EQ(ReadRegS(reg::GPRF::X1), -3);
    EXPECT_EQ(ReadRegS(reg::GPRF::X2), 3);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 8);
}

TEST_F(RV64IExecutorTest, ADDI) {
    LoadImm12(reg::GPRF::X1, -3);
    SetupInstrIS(InstructionOp::ADDI, reg::GPRF::X2, reg::GPRF::X1, -5);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(reg::GPRF::X2), -8);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLTI_True) {
    LoadImm12(reg::GPRF::X1, -3);
    SetupInstrIS(InstructionOp::SLTI, reg::GPRF::X2, reg::GPRF::X1, -2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLTI_False) {
    LoadImm12(reg::GPRF::X1, -2);
    SetupInstrIS(InstructionOp::SLTI, reg::GPRF::X2, reg::GPRF::X1, -3);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 0);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLTIU_True) {
    LoadImm12(reg::GPRF::X1, 2);
    SetupInstrIS(InstructionOp::SLTIU, reg::GPRF::X2, reg::GPRF::X1, 3);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLTIU_False) {
    LoadImm12(reg::GPRF::X1, 3);
    SetupInstrIS(InstructionOp::SLTIU, reg::GPRF::X2, reg::GPRF::X1, 2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 0);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, ANDI) {
    LoadImm12(reg::GPRF::X1, -1);
    SetupInstrIS(InstructionOp::ANDI, reg::GPRF::X2, reg::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, ORI) {
    LoadImm12(reg::GPRF::X1, -2);
    SetupInstrIS(InstructionOp::ORI, reg::GPRF::X2, reg::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(reg::GPRF::X2), -1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, XORI) {
    LoadImm12(reg::GPRF::X1, -1);
    SetupInstrIS(InstructionOp::XORI, reg::GPRF::X2, reg::GPRF::X1, 2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(reg::GPRF::X2), -3);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLLI) {
    LoadImm12(reg::GPRF::X1, 2);
    SetupInstrI(InstructionOp::SLLI, reg::GPRF::X2, reg::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 4);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRLI) {
    LoadImm12(reg::GPRF::X1, 4);
    SetupInstrI(InstructionOp::SRLI, reg::GPRF::X2, reg::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), 2);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRAI) {
    LoadImm12(reg::GPRF::X1, -2);
    SetupInstrI(InstructionOp::SRAI, reg::GPRF::X2, reg::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(reg::GPRF::X2), -1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, LUI) {
    constexpr const RV64UDWord IMM = 0b10101010101010101010;
    SetupInstrU(InstructionOp::LUI, reg::GPRF::X2, IMM);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), IMM << 12);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, AUIPC) {
    constexpr const RV64UDWord IMM = 0b10101010101010101010;
    SetupInstrU(InstructionOp::AUIPC, reg::GPRF::X2, IMM);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X2), (IMM << 12));
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, ADD) {
    LoadImm12(reg::GPRF::X1, 5);
    LoadImm12(reg::GPRF::X2, -2);
    SetupInstrR(InstructionOp::ADD, reg::GPRF::X3, reg::GPRF::X2,
                reg::GPRF::X1);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 3);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLT_True) {
    LoadImm12(reg::GPRF::X1, -3);
    LoadImm12(reg::GPRF::X2, -2);
    SetupInstrR(InstructionOp::SLT, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLT_False) {
    LoadImm12(reg::GPRF::X1, -2);
    LoadImm12(reg::GPRF::X2, -3);
    SetupInstrR(InstructionOp::SLT, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 0);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLTU_True) {
    LoadImm12(reg::GPRF::X1, 2);
    LoadImm12(reg::GPRF::X2, 3);
    SetupInstrR(InstructionOp::SLTU, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLTU_False) {
    LoadImm12(reg::GPRF::X1, 3);
    LoadImm12(reg::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SLTU, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 0);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLL) {
    LoadImm12(reg::GPRF::X1, 2);
    LoadImm12(reg::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SLL, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 8);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRL) {
    LoadImm12(reg::GPRF::X1, 8);
    LoadImm12(reg::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SRL, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X3), 2);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SUB) {
    LoadImm12(reg::GPRF::X1, -4);
    LoadImm12(reg::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SUB, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(reg::GPRF::X3), -6);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRA) {
    LoadImm12(reg::GPRF::X1, -4);
    LoadImm12(reg::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SRA, reg::GPRF::X3, reg::GPRF::X1,
                reg::GPRF::X2);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(reg::GPRF::X3), -1);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, JAL) {
    SetupInstrU(InstructionOp::JAL, reg::GPRF::X1, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X1), prevPC + 4);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), 15 * 2);
}

TEST_F(RV64IExecutorTest, JALR) {
    LoadImm12(reg::GPRF::X2, 11);
    SetupInstrI(InstructionOp::JALR, reg::GPRF::X1, reg::GPRF::X2, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::X1), prevPC + 4);
    EXPECT_EQ(ReadReg(reg::GPRF::PC), 11 + 15 * 2 - 1);
}

TEST_F(RV64IExecutorTest, BEQ_True) {
    LoadImm12(reg::GPRF::X2, 15);
    LoadImm12(reg::GPRF::X3, 15);
    SetupInstrB(InstructionOp::BEQ, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BEQ_False) {
    LoadImm12(reg::GPRF::X2, 15);
    LoadImm12(reg::GPRF::X3, 16);
    SetupInstrB(InstructionOp::BEQ, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BNE_True) {
    LoadImm12(reg::GPRF::X2, 15);
    LoadImm12(reg::GPRF::X3, 16);
    SetupInstrB(InstructionOp::BNE, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BNE_False) {
    LoadImm12(reg::GPRF::X2, 15);
    LoadImm12(reg::GPRF::X3, 15);
    SetupInstrB(InstructionOp::BNE, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BLT_True) {
    LoadImm12(reg::GPRF::X2, -3);
    LoadImm12(reg::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BLT, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BLT_False) {
    LoadImm12(reg::GPRF::X2, -2);
    LoadImm12(reg::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BLT, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BLTU_True) {
    LoadImm12(reg::GPRF::X2, 2);
    LoadImm12(reg::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BLTU, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BLTU_False) {
    LoadImm12(reg::GPRF::X2, 3);
    LoadImm12(reg::GPRF::X3, 2);
    SetupInstrB(InstructionOp::BLTU, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGE_Greater_True) {
    LoadImm12(reg::GPRF::X2, -2);
    LoadImm12(reg::GPRF::X3, -3);
    SetupInstrB(InstructionOp::BGE, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGE_Greater_False) {
    LoadImm12(reg::GPRF::X2, -3);
    LoadImm12(reg::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BGE, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGE_Equal_True) {
    LoadImm12(reg::GPRF::X2, -3);
    LoadImm12(reg::GPRF::X3, -3);
    SetupInstrB(InstructionOp::BGE, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGE_Equal_False) {
    LoadImm12(reg::GPRF::X2, -3);
    LoadImm12(reg::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BGE, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGEU_Greater_True) {
    LoadImm12(reg::GPRF::X2, 3);
    LoadImm12(reg::GPRF::X3, 2);
    SetupInstrB(InstructionOp::BGEU, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGEU_Greater_False) {
    LoadImm12(reg::GPRF::X2, 2);
    LoadImm12(reg::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BGEU, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGEU_Equal_True) {
    LoadImm12(reg::GPRF::X2, 3);
    LoadImm12(reg::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BGEU, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGEU_Equal_False) {
    LoadImm12(reg::GPRF::X2, 2);
    LoadImm12(reg::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BGEU, reg::GPRF::X2, reg::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(reg::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(reg::GPRF::PC), prevPC + 4);
}
