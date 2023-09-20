#include "./besm-666/decoder/decoder.hpp"
#include "besm-666/autogen/encoding.out.h"
#include <gtest/gtest.h>

using namespace besm;

class DecoderCommon : public ::testing::Test {
protected:
    const besm::dec::Decoder decoder{};

    [[nodiscard]] virtual bool equal(Instruction lhs, Instruction rhs) = 0;
    void SetUp() override {}

    void TearDown() override {}
};

class Decoder_R : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation && lhs.rd == rhs.rd &&
               lhs.rs1 == rhs.rs1 && lhs.rs2 == rhs.rs2;
    }
};

class Decoder_U : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation && lhs.immidiate == rhs.immidiate;
    }

    static Instruction buildInstr(Register rd, Immidiate imm,
                                  InstructionOp operation) {
        return {.rd = rd,
                .rs1 = 0b0,
                .rs2 = 0b0,
                .immidiate = imm,
                .operation = operation};
    }
};

TEST_F(Decoder_R, decodes_ADD) {
    auto instance =
        besm::Instruction{.rd = 0b11111,
                          .rs1 = 0b11111,
                          .rs2 = 0b11111,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::ADD};
    besm::Instruction parsed =
        decoder.parse(0b00000001111111111000111110110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SUB) {
    auto instance =
        besm::Instruction{.rd = 0b00010,
                          .rs1 = 0b10011,
                          .rs2 = 0b01111,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SUB};
    besm::Instruction parsed =
        decoder.parse(0b01000000111110011000000100110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SLL) {
    auto instance =
        besm::Instruction{.rd = 0b10000,
                          .rs1 = 0b01011,
                          .rs2 = 0b01011,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SLL};
    besm::Instruction parsed =
        decoder.parse(0b00000000101101011001100000110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SLT) {
    auto instance =
        besm::Instruction{.rd = 0b11111,
                          .rs1 = 0b11111,
                          .rs2 = 0b11111,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SLT};
    besm::Instruction parsed =
        decoder.parse(0b00000001111111111010111110110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SLTU) {
    auto instance =
        besm::Instruction{.rd = 0b00011,
                          .rs1 = 0b10011,
                          .rs2 = 0b00000,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SLTU};
    besm::Instruction parsed =
        decoder.parse(0b00000000000010011011000110110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_XOR) {
    auto instance =
        besm::Instruction{.rd = 0b00010,
                          .rs1 = 0b10011,
                          .rs2 = 0b11000,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::XOR};
    besm::Instruction parsed =
        decoder.parse(0b00000001100010011100000100110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SRL) {
    auto instance =
        besm::Instruction{.rd = 0b01010,
                          .rs1 = 0b10011,
                          .rs2 = 0b00111,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SRL};
    besm::Instruction parsed =
        decoder.parse(0b00000000011110011101010100110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SRA) {
    auto instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11011,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SRA};
    besm::Instruction parsed =
        decoder.parse(0b01000001101111011101110110110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_OR) {
    auto instance =
        besm::Instruction{.rd = 0b00100,
                          .rs1 = 0b11011,
                          .rs2 = 0b01011,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::OR};
    besm::Instruction parsed =
        decoder.parse(0b00000000101111011110001000110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_AND) {
    auto instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11011,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::AND};
    besm::Instruction parsed =
        decoder.parse(0b00000001101111011111110110110011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_ADDW) {
    auto instance =
        besm::Instruction{.rd = 0b10011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11010,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::ADDW};
    besm::Instruction parsed =
        decoder.parse(0b00000001101011011000100110111011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SUBW) {
    auto instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11011,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SUBW};
    besm::Instruction parsed =
        decoder.parse(0b01000001101111011000110110111011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SLLW) {
    auto instance =
        besm::Instruction{.rd = 0b01011,
                          .rs1 = 0b11011,
                          .rs2 = 0b10011,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SLLW};
    besm::Instruction parsed =
        decoder.parse(0b00000001001111011001010110111011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SRLW) {
    auto instance =
        besm::Instruction{.rd = 0b01000,
                          .rs1 = 0b11010,
                          .rs2 = 0b10001,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SRLW};
    besm::Instruction parsed =
        decoder.parse(0b00000001000111010101010000111011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SRAW) {
    auto instance =
        besm::Instruction{.rd = 0b00000,
                          .rs1 = 0b00101,
                          .rs2 = 0b00001,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SRAW};
    besm::Instruction parsed =
        decoder.parse(0b01000000000100101101000000111011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_EQ(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_R, decodes_SUBW_fails) {
    auto instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b1111,
                          .immidiate = Instruction::IMMIDIATE_POISON,
                          .operation = besm::SUBW};
    besm::Instruction parsed =
        decoder.parse(0b01000001101111011000110110111011);
    EXPECT_EQ(instance.operation, parsed.operation);
    EXPECT_EQ(instance.rs1, parsed.rs1);
    EXPECT_NE(instance.rs2, parsed.rs2);
    EXPECT_EQ(instance.rd, parsed.rd);
    EXPECT_FALSE(equal(parsed, instance));
}

TEST_F(Decoder_U, LUI1) {
    const auto instance = buildInstr(0b10000, 0b01000101001001001100, LUI);
    Instruction parsed = decoder.parse(0b01000101001001001100100000110111);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_U, LUI2) {
    const auto instance = buildInstr(0b01101, 0b01011011001110000100, LUI);
    Instruction parsed = decoder.parse(0b01011011001110000100011010110111);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_U, AUIPC1) {
    const auto instance = buildInstr(0b11000, 0b01010101101111101101, AUIPC);
    Instruction parsed = decoder.parse(0b01010101101111101101110000010111);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_U, AUIPC2) {
    const auto instance = buildInstr(0b00001, 0b10100100011000111010, AUIPC);
    Instruction parsed = decoder.parse(0b10100100011000111010000010010111);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_U, NOT_AUIPC) {
    const auto instance = buildInstr(0b00001, 0b00100100011000111010, AUIPC);
    // should  be 0b10100100011000111010, not 0b00100100011000111010
    Instruction parsed = decoder.parse(0b10100100011000111010000010010111);
    EXPECT_FALSE(equal(parsed, instance));
}
