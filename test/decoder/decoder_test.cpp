#include "./besm-666/decoder/decoder.hpp"
#include "besm-666/autogen/encoding.out.h"
#include "besm-666/instruction.hpp"
#include <gtest/gtest.h>

#define MATCH_AND 0x7033

TEST(Decoder, decodes_simple_instruction) { ASSERT_TRUE(true); }

TEST(Decoder_R, decodes_ADD) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b11111,
                          .rs1 = 0b11111,
                          .rs2 = 0b11111,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::ADD};
    besm::Instruction parsed =
        decoder.parse(0b00000001111111111000111110110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SUB) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b00010,
                          .rs1 = 0b10011,
                          .rs2 = 0b01111,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SUB};
    besm::Instruction parsed =
        decoder.parse(0b01000000111110011000000100110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SLL) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b10000,
                          .rs1 = 0b01011,
                          .rs2 = 0b01011,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SLL};
    besm::Instruction parsed =
        decoder.parse(0b00000000101101011001100000110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SLT) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b11111,
                          .rs1 = 0b11111,
                          .rs2 = 0b11111,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SLT};
    besm::Instruction parsed =
        decoder.parse(0b00000001111111111010111110110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SLTU) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b00011,
                          .rs1 = 0b10011,
                          .rs2 = 0b00000,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SLTU};
    besm::Instruction parsed =
        decoder.parse(0b00000000000010011011000110110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_XOR) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b00010,
                          .rs1 = 0b10011,
                          .rs2 = 0b11000,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::XOR};
    besm::Instruction parsed =
        decoder.parse(0b00000001100010011100000100110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SRL) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b01010,
                          .rs1 = 0b10011,
                          .rs2 = 0b00111,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SRL};
    besm::Instruction parsed =
        decoder.parse(0b00000000011110011101010100110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SRA) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11011,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SRA};
    besm::Instruction parsed =
        decoder.parse(0b01000001101111011101110110110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_OR) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b00100,
                          .rs1 = 0b11011,
                          .rs2 = 0b01011,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::OR};
    besm::Instruction parsed =
        decoder.parse(0b00000000101111011110001000110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_AND) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11011,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::AND};
    besm::Instruction parsed =
        decoder.parse(0b00000001101111011111110110110011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_ADDW) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b10011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11010,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::ADDW};
    besm::Instruction parsed =
        decoder.parse(0b00000001101011011000100110111011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SUBW) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b11011,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SUBW};
    besm::Instruction parsed =
        decoder.parse(0b01000001101111011000110110111011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SLLW) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b01011,
                          .rs1 = 0b11011,
                          .rs2 = 0b10011,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SLLW};
    besm::Instruction parsed =
        decoder.parse(0b00000001001111011001010110111011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SRLW) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b01000,
                          .rs1 = 0b11010,
                          .rs2 = 0b10001,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SRLW};
    besm::Instruction parsed =
        decoder.parse(0b00000001000111010101010000111011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SRAW) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b00000,
                          .rs1 = 0b00101,
                          .rs2 = 0b00001,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SRAW};
    besm::Instruction parsed =
        decoder.parse(0b01000000000100101101000000111011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_EQ(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_EQ(parsed, add_instance);
}

TEST(Decoder_R, decodes_SUBW_fails) {
    besm::dec::Decoder decoder{};
    besm::Instruction add_instance =
        besm::Instruction{.rd = 0b11011,
                          .rs1 = 0b11011,
                          .rs2 = 0b1111,
                          .immidiate = besm::IMMIDIATE_POISON,
                          .operation = besm::SUBW};
    besm::Instruction parsed =
        decoder.parse(0b01000001101111011000110110111011);
    ASSERT_EQ(add_instance.operation, parsed.operation);
    ASSERT_EQ(add_instance.rs1, parsed.rs1);
    ASSERT_NE(add_instance.rs2, parsed.rs2);
    ASSERT_EQ(add_instance.rd, parsed.rd);
    ASSERT_NE(parsed, add_instance);
}

#undef MATCH_AND
