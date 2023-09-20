#include <gtest/gtest.h>

#include "besm-666/utils/elf-parser.hpp"
#include "elf-gen.hpp"

using namespace besm;

class ElfParserTest : public ::testing::Test {
protected:
    std::filesystem::path elfPath = "./generated_elf";
};

TEST_F(ElfParserTest, requirements_test) {
    using namespace besm::utl;

    gen::generateSuitableElf(elfPath);
    EXPECT_NO_THROW(ElfParser(elfPath).getLoadableSegments());

    gen::generateUnsuitableElf(elfPath, ELFIO::ELFCLASS32,
                                        ELFIO::ELFDATA2LSB, ELFIO::EM_386);
    EXPECT_THROW(ElfParser(elfPath).getLoadableSegments(), BadElf);
}

TEST_F(ElfParserTest, get_loadable_segments_test) {
    using namespace besm::utl;

    static constexpr char segData[] = {'\x62', '\x6F', '\x6F', '\x62', '\x73', '\x00'};
    constexpr RV64Ptr ptr = 0x08048000;
    gen::generateSuitableElf(elfPath, segData, sizeof(segData), ptr);

    ElfParser parser = ElfParser(elfPath);
    for (const auto &seg : parser.getLoadableSegments()) {
        EXPECT_EQ(seg.size, sizeof(segData));
        EXPECT_EQ(seg.address, ptr);
        for (int i = 0; i < seg.size; i++) {
            EXPECT_EQ(*((char*)seg.data + i), segData[i]);
        }
    }
}