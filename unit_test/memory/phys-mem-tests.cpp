#include <gtest/gtest.h>

#include "besm-666/memory/phys-mem.hpp"
#include "elf-gen.hpp"

constexpr size_t RAMSize = 64ull * 1024 * 1024 * 1024; // 32GB
constexpr size_t PageSize = 4096;                      // 4KB
constexpr size_t ChunkSize = 1024 * 1024;              // 1MB

TEST(phys_mem_tests, load_store_byte) {
    using namespace besm::mem;

    std::shared_ptr<PhysMem> mem =
        PhysMemBuilder().mapRAM(0, RAMSize, PageSize, ChunkSize).build();

    mem->storeByte(0xBAD0BABE, 42);
    EXPECT_EQ(mem->loadByte(0xBAD0BABE), 42);
}

TEST(phys_mem_tests, cont_area) {
    using namespace besm::mem;

    constexpr char Area[] = "Hello, world!";
    constexpr besm::RV64Ptr Address = PageSize - 3;

    std::shared_ptr<PhysMem> mem =
        PhysMemBuilder().mapRAM(0, RAMSize, PageSize, ChunkSize).build();
    mem->storeContArea(Address, Area, sizeof(Area));

    for (besm::RV64Size i = 0; i < sizeof(Area); ++i) {
        EXPECT_EQ(mem->loadByte(Address + i), Area[i]);
    }
}

TEST(phys_mem_tests, load_elf) {
    using namespace besm::mem;
    using namespace besm;

    std::filesystem::path elfPath = "./generated_elf";
    gen::generateSuitableElf(elfPath);

    std::shared_ptr<PhysMem> memory =
        PhysMemBuilder().mapRAM(0, RAMSize, PageSize, ChunkSize).build();
    PhysMemLoader(memory).loadElf(elfPath);

    for (RV64Ptr i = 0; i < sizeof(gen::defaultData); i++) {
        EXPECT_EQ(memory->loadByte(gen::defaultPtr + i), gen::defaultData[i]);
    }
}
