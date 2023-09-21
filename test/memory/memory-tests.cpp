#include <gtest/gtest.h>

#include "besm-666/memory/memory.hpp"

using namespace besm;

TEST(memory_tests, storeByte) {
    mem::MemoryBuilder builder(1024 * 4, 1024 * 1024 * 2);
    mem::Memory memory = builder.build();

    constexpr RV64Ptr const ADDR = 1000;
    constexpr RV64UDWord const VAL = 0xDEADBABEBAD0BEEF;

    memory.storeByte(ADDR, static_cast<RV64UChar>(VAL));
    EXPECT_EQ(memory.loadByte(ADDR), static_cast<RV64UChar>(VAL));

    memory.storeHWord(ADDR, static_cast<RV64UHWord>(VAL));
    EXPECT_EQ(memory.loadHWord(ADDR), static_cast<RV64UHWord>(VAL));

    memory.storeWord(ADDR, static_cast<RV64UWord>(VAL));
    EXPECT_EQ(memory.loadWord(ADDR), static_cast<RV64UWord>(VAL));

    memory.storeDWord(ADDR, static_cast<RV64UDWord>(VAL));
    EXPECT_EQ(memory.loadDWord(ADDR), static_cast<RV64UDWord>(VAL));
}
