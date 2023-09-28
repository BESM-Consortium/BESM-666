#include <gtest/gtest.h>

#include "besm-666/exec/executor.hpp"
#include "besm-666/exec/gprf.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/util/bit-magic.hpp"

using namespace besm;

class RV64IExecutorTest : public testing::Test {
public:
    RV64IExecutorTest()
        : pMem(mem::PhysMemBuilder(4096, 2 * 1024 * 1024).build()),
          mmu(mem::MMU::Create(pMem)), exec(mmu) {}

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
        instr.immidiate = util::ExtractBits<RV64UDWord, 12>(imm);
    }
    void SetupInstrISigned(InstructionOp op, Register rd, Register rs1,
                           RV64DWord imm) {
        SetupInstrI(op, rd, rs1, util::Unsignify(imm));
    }
    void SetupInstrU(InstructionOp op, Register rd, RV64UDWord imm) {
        instr.operation = op;
        instr.rd = rd;
        instr.immidiate = util::ExtractBits<RV64UDWord, 20>(imm);
    }
    void SetupInstrB(InstructionOp op, Register rs1, Register rs2,
                     RV64UDWord imm) {
        instr.operation = op;
        instr.rs1 = rs1;
        instr.rs2 = rs2;
        instr.immidiate = imm;
    }

    void LoadImm12(Register rd, RV64DWord val) {
        SetupInstrISigned(InstructionOp::ADDI, rd, exec::GPRF::X0, val);
        Exec();
    }

    RV64UDWord ReadReg(Register r) { return exec.getState().read(r); }
    RV64DWord ReadRegS(Register r) {
        return util::Signify(exec.getState().read(r));
    }

    void Exec() { exec.exec(instr); }

    Instruction instr;
    mem::PhysMem::SPtr pMem;
    mem::MMU::SPtr mmu;
    exec::Executor exec;
};

TEST_F(RV64IExecutorTest, MOVI) {
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    LoadImm12(exec::GPRF::X1, -3);
    LoadImm12(exec::GPRF::X2, 3);

    EXPECT_EQ(ReadRegS(exec::GPRF::X1), -3);
    EXPECT_EQ(ReadRegS(exec::GPRF::X2), 3);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 8);
}

TEST_F(RV64IExecutorTest, ADDI) {
    LoadImm12(exec::GPRF::X1, -3);
    SetupInstrISigned(InstructionOp::ADDI, exec::GPRF::X2, exec::GPRF::X1, -5);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(exec::GPRF::X2), -8);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLTI_True) {
    LoadImm12(exec::GPRF::X1, -3);
    SetupInstrISigned(InstructionOp::SLTI, exec::GPRF::X2, exec::GPRF::X1, -2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLTI_False) {
    LoadImm12(exec::GPRF::X1, -2);
    SetupInstrISigned(InstructionOp::SLTI, exec::GPRF::X2, exec::GPRF::X1, -3);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 0);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLTIU_True) {
    LoadImm12(exec::GPRF::X1, 2);
    SetupInstrISigned(InstructionOp::SLTIU, exec::GPRF::X2, exec::GPRF::X1, 3);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLTIU_False) {
    LoadImm12(exec::GPRF::X1, 3);
    SetupInstrISigned(InstructionOp::SLTIU, exec::GPRF::X2, exec::GPRF::X1, 2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 0);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, ANDI) {
    LoadImm12(exec::GPRF::X1, -1);
    SetupInstrISigned(InstructionOp::ANDI, exec::GPRF::X2, exec::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, ORI) {
    LoadImm12(exec::GPRF::X1, -2);
    SetupInstrISigned(InstructionOp::ORI, exec::GPRF::X2, exec::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(exec::GPRF::X2), -1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, XORI) {
    LoadImm12(exec::GPRF::X1, -1);
    SetupInstrISigned(InstructionOp::XORI, exec::GPRF::X2, exec::GPRF::X1, 2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(exec::GPRF::X2), -3);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLLI) {
    LoadImm12(exec::GPRF::X1, 2);
    SetupInstrI(InstructionOp::SLLI, exec::GPRF::X2, exec::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 4);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRLI) {
    LoadImm12(exec::GPRF::X1, 4);
    SetupInstrI(InstructionOp::SRLI, exec::GPRF::X2, exec::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), 2);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRAI) {
    LoadImm12(exec::GPRF::X1, -2);
    SetupInstrI(InstructionOp::SRAI, exec::GPRF::X2, exec::GPRF::X1, 1);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(exec::GPRF::X2), -1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, LUI) {
    constexpr const RV64UDWord IMM = 0b10101010101010101010;
    SetupInstrU(InstructionOp::LUI, exec::GPRF::X2, IMM);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), IMM << 12);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, AUIPC) {
    constexpr const RV64UDWord IMM = 0b10101010101010101010;
    SetupInstrU(InstructionOp::AUIPC, exec::GPRF::X2, IMM);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X2), (IMM << 12));
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, ADD) {
    LoadImm12(exec::GPRF::X1, 5);
    LoadImm12(exec::GPRF::X2, -2);
    SetupInstrR(InstructionOp::ADD, exec::GPRF::X3, exec::GPRF::X2,
                exec::GPRF::X1);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 3);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLT_True) {
    LoadImm12(exec::GPRF::X1, -3);
    LoadImm12(exec::GPRF::X2, -2);
    SetupInstrR(InstructionOp::SLT, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLT_False) {
    LoadImm12(exec::GPRF::X1, -2);
    LoadImm12(exec::GPRF::X2, -3);
    SetupInstrR(InstructionOp::SLT, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 0);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLTU_True) {
    LoadImm12(exec::GPRF::X1, 2);
    LoadImm12(exec::GPRF::X2, 3);
    SetupInstrR(InstructionOp::SLTU, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}
TEST_F(RV64IExecutorTest, SLTU_False) {
    LoadImm12(exec::GPRF::X1, 3);
    LoadImm12(exec::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SLTU, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 0);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SLL) {
    LoadImm12(exec::GPRF::X1, 2);
    LoadImm12(exec::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SLL, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 8);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRL) {
    LoadImm12(exec::GPRF::X1, 8);
    LoadImm12(exec::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SRL, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X3), 2);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SUB) {
    LoadImm12(exec::GPRF::X1, -4);
    LoadImm12(exec::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SUB, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(exec::GPRF::X3), -6);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, SRA) {
    LoadImm12(exec::GPRF::X1, -4);
    LoadImm12(exec::GPRF::X2, 2);
    SetupInstrR(InstructionOp::SRA, exec::GPRF::X3, exec::GPRF::X1,
                exec::GPRF::X2);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadRegS(exec::GPRF::X3), -1);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, JAL) {
    SetupInstrU(InstructionOp::JAL, exec::GPRF::X1, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X1), prevPC + 4);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), 15 * 2);
}

TEST_F(RV64IExecutorTest, JALR) {
    LoadImm12(exec::GPRF::X2, 11);
    SetupInstrI(InstructionOp::JALR, exec::GPRF::X1, exec::GPRF::X2, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::X1), prevPC + 4);
    EXPECT_EQ(ReadReg(exec::GPRF::PC), 11 + 15 * 2 - 1);
}

TEST_F(RV64IExecutorTest, BEQ_True) {
    LoadImm12(exec::GPRF::X2, 15);
    LoadImm12(exec::GPRF::X3, 15);
    SetupInstrB(InstructionOp::BEQ, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BEQ_False) {
    LoadImm12(exec::GPRF::X2, 15);
    LoadImm12(exec::GPRF::X3, 16);
    SetupInstrB(InstructionOp::BEQ, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BNE_True) {
    LoadImm12(exec::GPRF::X2, 15);
    LoadImm12(exec::GPRF::X3, 16);
    SetupInstrB(InstructionOp::BNE, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BNE_False) {
    LoadImm12(exec::GPRF::X2, 15);
    LoadImm12(exec::GPRF::X3, 15);
    SetupInstrB(InstructionOp::BNE, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BLT_True) {
    LoadImm12(exec::GPRF::X2, -3);
    LoadImm12(exec::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BLT, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BLT_False) {
    LoadImm12(exec::GPRF::X2, -2);
    LoadImm12(exec::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BLT, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BLTU_True) {
    LoadImm12(exec::GPRF::X2, 2);
    LoadImm12(exec::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BLTU, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BLTU_False) {
    LoadImm12(exec::GPRF::X2, 3);
    LoadImm12(exec::GPRF::X3, 2);
    SetupInstrB(InstructionOp::BLTU, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGE_Greater_True) {
    LoadImm12(exec::GPRF::X2, -2);
    LoadImm12(exec::GPRF::X3, -3);
    SetupInstrB(InstructionOp::BGE, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGE_Greater_False) {
    LoadImm12(exec::GPRF::X2, -3);
    LoadImm12(exec::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BGE, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGE_Equal_True) {
    LoadImm12(exec::GPRF::X2, -3);
    LoadImm12(exec::GPRF::X3, -3);
    SetupInstrB(InstructionOp::BGE, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGE_Equal_False) {
    LoadImm12(exec::GPRF::X2, -3);
    LoadImm12(exec::GPRF::X3, -2);
    SetupInstrB(InstructionOp::BGE, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGEU_Greater_True) {
    LoadImm12(exec::GPRF::X2, 3);
    LoadImm12(exec::GPRF::X3, 2);
    SetupInstrB(InstructionOp::BGEU, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGEU_Greater_False) {
    LoadImm12(exec::GPRF::X2, 2);
    LoadImm12(exec::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BGEU, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, BGEU_Equal_True) {
    LoadImm12(exec::GPRF::X2, 3);
    LoadImm12(exec::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BGEU, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 30);
}
TEST_F(RV64IExecutorTest, BGEU_Equal_False) {
    LoadImm12(exec::GPRF::X2, 2);
    LoadImm12(exec::GPRF::X3, 3);
    SetupInstrB(InstructionOp::BGEU, exec::GPRF::X2, exec::GPRF::X3, 15);
    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4);
}

TEST_F(RV64IExecutorTest, LOAD_Positive) {
    constexpr const RV64UDWord BASE = 42;
    constexpr const RV64UDWord OFFSET = 10;
    constexpr const RV64UDWord ADDRESS = BASE + OFFSET;

    LoadImm12(exec::GPRF::X2, BASE);
    mmu->storeDWord(ADDRESS, 100);

    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    SetupInstrI(InstructionOp::LB, exec::GPRF::X3, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LH, exec::GPRF::X4, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LW, exec::GPRF::X5, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LD, exec::GPRF::X6, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LBU, exec::GPRF::X7, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LHU, exec::GPRF::X8, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LWU, exec::GPRF::X9, exec::GPRF::X2, OFFSET);
    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4 * 7);
    EXPECT_EQ(ReadReg(exec::GPRF::X3), 100);
    EXPECT_EQ(ReadReg(exec::GPRF::X4), 100);
    EXPECT_EQ(ReadReg(exec::GPRF::X5), 100);
    EXPECT_EQ(ReadReg(exec::GPRF::X6), 100);
    EXPECT_EQ(ReadReg(exec::GPRF::X7), 100);
    EXPECT_EQ(ReadReg(exec::GPRF::X8), 100);
    EXPECT_EQ(ReadReg(exec::GPRF::X9), 100);
}

TEST_F(RV64IExecutorTest, LOAD_Negative) {
    constexpr const RV64UDWord BASE = 42;
    constexpr const RV64UDWord OFFSET = 10;
    constexpr const RV64UDWord ADDRESS = BASE + OFFSET;

    LoadImm12(exec::GPRF::X2, BASE);
    mmu->storeDWord(ADDRESS, -1);

    RV64UDWord prevPC = ReadReg(exec::GPRF::PC);

    SetupInstrI(InstructionOp::LB, exec::GPRF::X3, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LH, exec::GPRF::X4, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LW, exec::GPRF::X5, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LD, exec::GPRF::X6, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LBU, exec::GPRF::X7, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LHU, exec::GPRF::X8, exec::GPRF::X2, OFFSET);
    Exec();
    SetupInstrI(InstructionOp::LWU, exec::GPRF::X9, exec::GPRF::X2, OFFSET);
    Exec();

    EXPECT_EQ(ReadReg(exec::GPRF::PC), prevPC + 4 * 7);
    EXPECT_EQ(ReadReg(exec::GPRF::X3), util::Unsignify<RV64DWord>(-1));
    EXPECT_EQ(ReadReg(exec::GPRF::X4), util::Unsignify<RV64DWord>(-1));
    EXPECT_EQ(ReadReg(exec::GPRF::X5), util::Unsignify<RV64DWord>(-1));
    EXPECT_EQ(ReadReg(exec::GPRF::X6), util::Unsignify<RV64DWord>(-1));
    EXPECT_EQ(ReadReg(exec::GPRF::X7), std::numeric_limits<RV64UChar>::max());
    EXPECT_EQ(ReadReg(exec::GPRF::X8), std::numeric_limits<RV64UHWord>::max());
    EXPECT_EQ(ReadReg(exec::GPRF::X9), std::numeric_limits<RV64UWord>::max());
}
