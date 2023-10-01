#include <gtest/gtest.h>

#include "besm-666/util/elf-parser.hpp"
#include "elf-gen.hpp"

using namespace besm;

class ElfParserTest : public ::testing::Test {
protected:
    std::filesystem::path elfPath = "./generated_elf";
};

TEST_F(ElfParserTest, requirements_true) {
    using namespace besm::util;

    gen::generateSuitableElf(elfPath);
    EXPECT_NO_THROW(createParser(elfPath));
}

TEST_F(ElfParserTest, requirements_false) {
    using namespace besm::util;

    gen::generateUnsuitableElf(elfPath, ELFIO::ELFCLASS32, ELFIO::ELFDATA2LSB,
                               ELFIO::EM_386);
    EXPECT_THROW(createParser(elfPath), UnavailableELFRequirements);
}

TEST_F(ElfParserTest, get_loadable_segments) {
    using namespace besm::util;

    gen::generateSuitableElf(elfPath);

    auto parser = createParser(elfPath);
    for (const auto &seg : parser->getLoadableSegments()) {
        EXPECT_EQ(seg.size, sizeof(gen::defaultData));
        EXPECT_EQ(seg.address, gen::defaultPtr);
        for (int i = 0; i < seg.size; i++) {
            EXPECT_EQ(*((char const *)seg.data + i), gen::defaultData[i]);
        }
    }
}
