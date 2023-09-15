#include <gtest/gtest.h>

#include "besm-666/memory/phys-mem.hpp"

constexpr size_t PageSize = 4096;
constexpr size_t ChunkSize = 1024 * 1024;

TEST(phys_mem_tests, load_store_byte) {
    using namespace besm::mem;

    PhysMem mem = PhysMemBuilder(PageSize, ChunkSize).build();

    mem.store<char>(0xBAD0BABE, 42);
    EXPECT_EQ(mem.load<char>(0xBAD0BABE), 42);
}

TEST(phys_mem_tests, cont_area) {
    using namespace besm::mem;

    constexpr char Area[] = "Hello, world!";
    constexpr besm::RV64Ptr Address = PageSize - 3;

    PhysMem mem = PhysMemBuilder(PageSize, ChunkSize)
                      .loadContArea(Address, Area, sizeof(Area))
                      .build();

    for (besm::RV64Size i = 0; i < sizeof(Area); ++i) {
        EXPECT_EQ(mem.load<char>(Address + i), Area[i]);
    }
}