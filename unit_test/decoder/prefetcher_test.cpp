#include "./besm-666/decoder/prefetcher.hpp"
#include "besm-666/autogen/encoding.out.h"
#include <gtest/gtest.h>

using namespace besm;

TEST(Prefecher, fetch_not_fails) {
    constexpr size_t RAM_SIZE = 1024 * 1024 * 1024;
    std::shared_ptr<mem::PhysMem> pMem =
        mem::PhysMemBuilder()
            .mapRAM(0, RAM_SIZE, 4096, 2 * 1024 * 1024)
            .build();
    mem::MMU::SPtr mmu = mem::MMU::Create(pMem);
    dec::Prefetcher prefetcher{mmu};
    constexpr RV64Ptr const ADDR = 1000;

    constexpr int to_store = 4096;
    for (int i = 0; i < to_store; ++i) {
        mmu->storeWord(ADDR + i * sizeof(RV64UWord), i);
    }
    for (int i = 0; i < to_store; ++i) {
        RV64UWord word = prefetcher.loadWord(ADDR + i * sizeof(RV64UWord));
        EXPECT_EQ(word, i);
    }
}
