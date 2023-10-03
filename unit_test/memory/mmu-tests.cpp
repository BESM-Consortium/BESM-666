#include <gtest/gtest.h>

#include "besm-666/memory/mmu.hpp"
#include "besm-666/memory/phys-mem.hpp"

using namespace besm;

TEST(mmu_tests, storeByte) {
    mem::PhysMem::SPtr pMem = mem::PhysMemBuilder(4096).build();
    mem::MMU::SPtr mmu = mem::MMU::Create(pMem);

    constexpr RV64Ptr const ADDR = 1000;
    constexpr RV64UDWord const VAL = 0xDEADBABEBAD0BEEF;

    mmu->storeByte(ADDR, static_cast<RV64UChar>(VAL));
    EXPECT_EQ(mmu->loadByte(ADDR), static_cast<RV64UChar>(VAL));

    mmu->storeHWord(ADDR, static_cast<RV64UHWord>(VAL));
    EXPECT_EQ(mmu->loadHWord(ADDR), static_cast<RV64UHWord>(VAL));

    mmu->storeWord(ADDR, static_cast<RV64UWord>(VAL));
    EXPECT_EQ(mmu->loadWord(ADDR), static_cast<RV64UWord>(VAL));

    mmu->storeDWord(ADDR, static_cast<RV64UDWord>(VAL));
    EXPECT_EQ(mmu->loadDWord(ADDR), static_cast<RV64UDWord>(VAL));
}
