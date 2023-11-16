#include <gtest/gtest.h>
#include <iostream>
#include <utility>

#include "besm-666/exec/gprf.hpp"
#include "besm-666/instruction.hpp"
#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"
#include "besm-666/sim/hart.hpp"
#include "besm-666/sim/hooks.hpp"
#include "besm-666/util/bit-magic.hpp"

using namespace besm;

/*
 * int accum = 0;
 * for(int i = 0; i < 5; ++i) {
 *      accum += i
 * }
 *
 * X1 - accum
 * X2 - iterator (i)
 * X3 - upper limit (5)
 */

TEST(Executor, Sum) {
    Instruction instrs[] = {
        // PC = 0 - sets up accum to zero
        {exec::GPRF::X1, exec::GPRF::X0, exec::GPRF::X0, 0, InstructionOp::ADD},
        // PC = 4 - sets up iterator to zero
        {exec::GPRF::X2, exec::GPRF::X0, exec::GPRF::X0, 0, InstructionOp::ADD},
        // PC = 8 - set up upper limit of for cycle
        {exec::GPRF::X3, exec::GPRF::X0, exec::GPRF::X0, 5,
         InstructionOp::ADDI},
        // PC = 12 - add iterator value to accum
        {exec::GPRF::X1, exec::GPRF::X1, exec::GPRF::X2, 0, InstructionOp::ADD},
        // PC = 16 - increment iterator
        {exec::GPRF::X2, exec::GPRF::X2, exec::GPRF::X0, 1,
         InstructionOp::ADDI},
        // PC = 20 - branch if iterator less than upper limit
        {exec::GPRF::X0, exec::GPRF::X2, exec::GPRF::X3, util::Unsignify(-8),
         InstructionOp::BLT},
        // PC = 24 - terminating nop
        {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
         InstructionOp::ADD}};

    std::shared_ptr<mem::PhysMem> pMem = mem::PhysMemBuilder().build();
    sim::HookManager::SPtr hookManager = sim::HookManager::Create();
    sim::Hart::SPtr hart = sim::Hart::Create(pMem, hookManager);
    RV64UDWord pc = hart->getGPRF().read(exec::GPRF::PC);

    while (pc <= 20) {
        hart->exec(instrs[pc / 4]);
        pc = hart->getGPRF().read(exec::GPRF::PC);
    }

    EXPECT_EQ(hart->getGPRF().read(exec::GPRF::X1), 10);
}

/*
 * X1 - first factor
 * X2 - second factor
 * X3 - result
 * X4 - bitness (32)
 * X5 - iterator
 */

TEST(Executor, Mul) {
    Instruction instrs[] = {
        // PC = 0 - set up 1st factor to 15
        {exec::GPRF::X1, exec::GPRF::X0, exec::GPRF::X0, 15,
         InstructionOp::ADDI},
        // PC = 4 - set up 2nd factor to 8
        {exec::GPRF::X2, exec::GPRF::X0, exec::GPRF::X0, 8,
         InstructionOp::ADDI},
        // PC = 8 - set up result to zero
        {exec::GPRF::X3, exec::GPRF::X0, exec::GPRF::X0, 0, InstructionOp::ADD},
        // PC = 12 - set up bitness to 32
        {exec::GPRF::X4, exec::GPRF::X0, exec::GPRF::X0, 32,
         InstructionOp::ADDI},
        // PC = 16 - set up iterator to zero
        {exec::GPRF::X5, exec::GPRF::X0, exec::GPRF::X0, 0, InstructionOp::ADD},
        // PC = 20 - get zero bit of second factor
        {exec::GPRF::X6, exec::GPRF::X2, exec::GPRF::X0, 1,
         InstructionOp::ANDI},
        // PC = 24 - skip adding if bit is not set
        {exec::GPRF::X0, exec::GPRF::X6, exec::GPRF::X0, 8, InstructionOp::BEQ},
        // PC = 28 - add first factor to result is bit is set
        {exec::GPRF::X3, exec::GPRF::X3, exec::GPRF::X1, 0, InstructionOp::ADD},
        // PC = 32 - shift bits of second factor right
        {exec::GPRF::X2, exec::GPRF::X2, exec::GPRF::X0, 1,
         InstructionOp::SRLI},
        // PC = 36 - shift bits of first factor left
        {exec::GPRF::X1, exec::GPRF::X1, exec::GPRF::X0, 1,
         InstructionOp::SLLI},
        // PC = 40 - continue if second factor is not zero
        {exec::GPRF::X0, exec::GPRF::X2, exec::GPRF::X0, util::Unsignify(-20),
         InstructionOp::BNE},
        // PC = 44 - terminating nop
        {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
         InstructionOp::ADD}};

    std::shared_ptr<mem::PhysMem> pMem = mem::PhysMemBuilder().build();
    sim::HookManager::SPtr hookManager = sim::HookManager::Create();
    sim::Hart::SPtr hart = sim::Hart::Create(pMem, hookManager);
    RV64UDWord pc = hart->getGPRF().read(exec::GPRF::PC);

    while (pc <= 40) {
        hart->exec(instrs[pc / 4]);
        pc = hart->getGPRF().read(exec::GPRF::PC);
    }

    EXPECT_EQ(hart->getGPRF().read(exec::GPRF::X3), 15 * 8);
}

class ExecutorBBTest : public ::testing::Test {
protected:
    void setInstrs(std::vector<Instruction> instrs) {
        instrs_ = std::move(instrs);
    }

    BasicBlock assembleBB() {
        RV64Ptr bbStartPC = hart_->getGPRF().read(exec::GPRF::PC);
        BasicBlock bb{bbStartPC};
        int i = 0;
        while (bb.put(instrs_[bbStartPC / sizeof(RV64UWord) -
                              startPC_ / sizeof(RV64UWord) + i])) {
            i++;
        }
        return bb;
    }

    std::vector<Instruction> instrs_;
    std::shared_ptr<mem::PhysMem> pMem_ = mem::PhysMemBuilder().build();
    sim::HookManager::SPtr hookManager = sim::HookManager::Create();
    sim::Hart::SPtr hart_ = sim::Hart::Create(pMem_, hookManager);
    RV64Ptr startPC_ = hart_->getGPRF().read(exec::GPRF::PC);
};

/*
 * int a = 5
 * int b = 10
 * int c = a + b
 *
 * X1 - a
 * X2 - b
 * X3 - c
 */

TEST_F(ExecutorBBTest, SingleBB) {
    setInstrs({{// PC = 0 - sets `a` to 5
                {exec::GPRF::X1, exec::GPRF::X0, exec::GPRF::X0, 5,
                 InstructionOp::ADDI},
                // PC = 4 - sets `b` to 10
                {exec::GPRF::X2, exec::GPRF::X0, exec::GPRF::X0, 10,
                 InstructionOp::ADDI},
                // PC = 8 - sets `b` to `a + c`
                {exec::GPRF::X3, exec::GPRF::X1, exec::GPRF::X2, 0,
                 InstructionOp::ADD},
                // PC = 12 - last instruction in basic block
                {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
                 InstructionOp::JAL}}});
    BasicBlock bb{0};
    bb = assembleBB();
    hart_->execBB(bb);

    EXPECT_EQ(hart_->getGPRF().read(exec::GPRF::X3), 15);
}

/*
 * int accum = 0;
 * for(int i = 0; i < 5; ++i) {
 *      accum += i
 * }
 *
 * X1 - accum
 * X2 - iterator (i)
 * X3 - upper limit (5)
 */

TEST_F(ExecutorBBTest, Sum) {
    setInstrs({// PC = 0 - sets up accum to zero
               {exec::GPRF::X1, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::ADD},
               // PC = 4 - sets up iterator to zero
               {exec::GPRF::X2, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::ADD},
               // PC = 8 - set up upper limit of for cycle
               {exec::GPRF::X3, exec::GPRF::X0, exec::GPRF::X0, 5,
                InstructionOp::ADDI},
               // PC = 12 - add iterator value to accum
               {exec::GPRF::X1, exec::GPRF::X1, exec::GPRF::X2, 0,
                InstructionOp::ADD},
               // PC = 16 - increment iterator
               {exec::GPRF::X2, exec::GPRF::X2, exec::GPRF::X0, 1,
                InstructionOp::ADDI},
               // PC = 20 - branch if iterator less than upper limit
               {exec::GPRF::X0, exec::GPRF::X2, exec::GPRF::X3,
                util::Unsignify(-8), InstructionOp::BLT},
               // PC = 24 - terminating nop
               {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::ADD},
               // PC = 28 - last instruction in basic block
               {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::JAL}});

    BasicBlock bb{0};
    while (bb.startPC() != 24) {
        bb = assembleBB();
        hart_->execBB(bb);
    }

    EXPECT_EQ(hart_->getGPRF().read(exec::GPRF::X1), 10);
}

/*
 * X1 - first factor
 * X2 - second factor
 * X3 - result
 * X4 - bitness (32)
 * X5 - iterator
 */

TEST_F(ExecutorBBTest, Mul) {
    setInstrs({// PC = 0 - set up 1st factor to 15
               {exec::GPRF::X1, exec::GPRF::X0, exec::GPRF::X0, 15,
                InstructionOp::ADDI},
               // PC = 4 - set up 2nd factor to 8
               {exec::GPRF::X2, exec::GPRF::X0, exec::GPRF::X0, 8,
                InstructionOp::ADDI},
               // PC = 8 - set up result to zero
               {exec::GPRF::X3, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::ADD},
               // PC = 12 - set up bitness to 32
               {exec::GPRF::X4, exec::GPRF::X0, exec::GPRF::X0, 32,
                InstructionOp::ADDI},
               // PC = 16 - set up iterator to zero
               {exec::GPRF::X5, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::ADD},
               // PC = 20 - get zero bit of second factor
               {exec::GPRF::X6, exec::GPRF::X2, exec::GPRF::X0, 1,
                InstructionOp::ANDI},
               // PC = 24 - skip adding if bit is not set
               {exec::GPRF::X0, exec::GPRF::X6, exec::GPRF::X0, 8,
                InstructionOp::BEQ},
               // PC = 28 - add first factor to result is bit is set
               {exec::GPRF::X3, exec::GPRF::X3, exec::GPRF::X1, 0,
                InstructionOp::ADD},
               // PC = 32 - shift bits of second factor right
               {exec::GPRF::X2, exec::GPRF::X2, exec::GPRF::X0, 1,
                InstructionOp::SRLI},
               // PC = 36 - shift bits of first factor left
               {exec::GPRF::X1, exec::GPRF::X1, exec::GPRF::X0, 1,
                InstructionOp::SLLI},
               // PC = 40 - continue if second factor is not zero
               {exec::GPRF::X0, exec::GPRF::X2, exec::GPRF::X0,
                util::Unsignify(-20), InstructionOp::BNE},
               // PC = 44 - terminating nop
               {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::ADD},
               // PC = 48 - last instruction in basic block
               {exec::GPRF::X0, exec::GPRF::X0, exec::GPRF::X0, 0,
                InstructionOp::JAL}});

    BasicBlock bb{0};
    while (bb.startPC() != 44) {
        bb = assembleBB();
        hart_->execBB(bb);
    }

    EXPECT_EQ(hart_->getGPRF().read(exec::GPRF::X3), 15 * 8);
}
