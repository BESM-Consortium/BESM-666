#include <gtest/gtest.h>

#include "besm-666/utils/elf-parser.hpp"
#include "elf-gen.hpp"

using namespace besm;

class ElfParserTest : public ::testing::Test {
protected:
    std::filesystem::path elfPath = "./generated_elf";
};

TEST_F(ElfParserTest, requirements_true) {
    using namespace besm::utl;

    gen::generateSuitableElf(elfPath);
    EXPECT_NO_THROW(ElfParser suitableParser(elfPath););
}

TEST_F(ElfParserTest, requirements_false) {
    using namespace besm::utl;

    gen::generateUnsuitableElf(elfPath, ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB,
                               ELFIO::EM_386);
    EXPECT_THROW(ElfParser unsuitableParser(elfPath), BadElf);
}

TEST_F(ElfParserTest, get_loadable_segments) {
    using namespace besm::utl;

    gen::generateSuitableElf(elfPath);

    ElfParser parser = ElfParser(elfPath);
    for (const auto &seg : parser.getLoadableSegments()) {
        EXPECT_EQ(seg.size, sizeof(gen::defaultData));
        EXPECT_EQ(seg.address, gen::defaultPtr);
        for (int i = 0; i < seg.size; i++) {
            EXPECT_EQ(*((char const *)seg.data + i), gen::defaultData[i]);
        }
    }
}
