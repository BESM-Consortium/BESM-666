#include <gtest/gtest.h>

#include "besm-666/exec/executor.hpp"

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
                     Immidiate imm) {
        instr.operation = op;
        instr.rd = rd;
        instr.rs1 = rs1;
        instr.immidiate = imm;
    }

    void LoadImm12(Register rd, Immidiate val) {}

    Instruction instr;
    exec::Executor exec;
};

/*
TEST_F(RV64IExecutorTest, ADDI) {
    SetupInstrI(InstructionOp::ADDI, reg::GPRF::X2, reg::GPRF::X1, 5);
    gprf.write(reg::GPRF::X1, 3);

    exec.exec(instr);

    EXPECT_EQ(gprf.read(reg::GPRF::X2), 8);
}

TEST_F(RV64IExecutorTest, MOV) {
    SetupInstrI(InstructionOp::ADDI, reg::GPRF::X2, reg::GPRF::X1, 0);
    gprf.write(reg::GPRF::X1, 5);

    exec.exec(instr);

    EXPECT_EQ(gprf.read(reg::GPRF::X2), gprf.read(reg::GPRF::X1));
}

TEST_F(RV64IExecutorTest, SLTI_True) {
    SetupInstrI(InstructionOp::SLTI, reg::GPRF::X2, reg::GPRF::X1, 3);
    gprf.write(reg::GPRF::X1, 2);

    exec.exec(instr);

    EXPECT_EQ(gprf.read(reg::GPRF::X2), 1);
}
TEST_F(RV64IExecutorTest, SLTI_False) {
    SetupInstrI(InstructionOp::SLTI, reg::GPRF::X2, reg::GPRF::X1, -1);
    gprf.write(reg::GPRF::X1, 2);

    exec.exec(instr);

    EXPECT_EQ(gprf.read(reg::GPRF::X2), 0);
}
*/
