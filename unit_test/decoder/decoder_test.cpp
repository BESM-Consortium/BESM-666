#include "./besm-666/decoder/decoder.hpp"
#include "besm-666/autogen/encoding.out.h"
#include <gtest/gtest.h>

using namespace besm;

class DecoderCommon : public ::testing::Test {
protected:
    const besm::dec::Decoder decoder{nullptr};

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

class Decoder_I : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation &&
               lhs.immidiate == rhs.immidiate && lhs.rs1 == rhs.rs1 &&
               lhs.rd == rhs.rd;
    }

    static Instruction buildInstr(Register rd, Register rs1, Immidiate imm,
                                  InstructionOp operation) {
        return {.rd = rd,
                .rs1 = rs1,
                .rs2 = 0b0,
                .immidiate = imm,
                .operation = operation};
    }
};

class Decoder_S : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation &&
               lhs.immidiate == rhs.immidiate && lhs.rs1 == rhs.rs1 &&
               lhs.rs2 == rhs.rs2;
    }

    static Instruction buildInstr(Register rs1, Register rs2, Immidiate imm,
                                  InstructionOp operation) {
        return {
            .rs1 = rs1, .rs2 = rs2, .immidiate = imm, .operation = operation};
    }
};

class Decoder_B : public Decoder_S {};

class Decoder_U : public DecoderCommon {
protected:
    bool equal(const Instruction lhs, const Instruction rhs) override {
        return lhs.operation == rhs.operation && lhs.immidiate == rhs.immidiate;
    }

    /**
     * Make U format instr.
     * @param rd Register.
     * @param shifted_imm Immediate, shifted to the left to avert 12 following
     *  zeros.
     * @param operation Instruction operation.
     * @return Instruction.
     */
    static Instruction buildInstr(Register rd, Immidiate shifted_imm,
                                  InstructionOp operation) {
        constexpr int IMM_SHIFT = 12;
        return {.rd = rd,
                .rs1 = 0b0,
                .rs2 = 0b0,
                .immidiate = shifted_imm << IMM_SHIFT,
                .operation = operation};
    }
};

class Decoder_J : public DecoderCommon {
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

TEST_F(Decoder_J, JAL1) {
    const auto instance = buildInstr(0b00000, 0b111111111111111111110, JAL);
    Instruction parsed = decoder.parse(0b11111111111111111111000001101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL2) {
    const auto instance = buildInstr(0b11011, 0b000000000000000000010, JAL);
    Instruction parsed = decoder.parse(0b00000000001000000000110111101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL3) {
    const auto instance = buildInstr(0b11011, 0b011001111000010111110, JAL);
    Instruction parsed = decoder.parse(0b00001011111011001111110111101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL4) {
    const auto instance = buildInstr(0b01110, 0b110111010001000010110, JAL);
    Instruction parsed = decoder.parse(0b10100001011010111010011101101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL5) {
    const auto instance = buildInstr(0b11011, 0b000000000100000000000, JAL);
    Instruction parsed = decoder.parse(0b00000000000100000000110111101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL6) {
    const auto instance = buildInstr(0b11011, 0b000000010000000000000, JAL);
    Instruction parsed = decoder.parse(0b00000000000000000010110111101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL7) {
    const auto instance = buildInstr(0b11011, 0b100000000000000000000, JAL);
    Instruction parsed = decoder.parse(0b10000000000000000000110111101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_J, JAL8) {
    const auto instance = buildInstr(0b11011, 0b010000000000000000000, JAL);
    Instruction parsed = decoder.parse(0b00000000000010000000110111101111);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, JALR1) {
    const auto instance = buildInstr(0b01011, 0b11101, 0b000111000000, JALR);
    Instruction parsed = decoder.parse(0b00011100000011101000010111100111);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, JALR2) {
    const auto instance = buildInstr(0b10010, 0b01101, 0b101011001010, JALR);
    Instruction parsed = decoder.parse(0b10101100101001101000100101100111);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LB) {
    const auto instance = buildInstr(0b10001, 0b11011, 0b111111100010, LB);
    Instruction parsed = decoder.parse(0b11111110001011011000100010000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LH) {
    const auto instance = buildInstr(0b10011, 0b11001, 0b011000101011, LH);
    Instruction parsed = decoder.parse(0b01100010101111001001100110000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LW) {
    const auto instance = buildInstr(0b01111, 0b01010, 0b100110101101, LW);
    Instruction parsed = decoder.parse(0b10011010110101010010011110000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LBU) {
    const auto instance = buildInstr(0b01000, 0b11111, 0b111110000001, LBU);
    Instruction parsed = decoder.parse(0b11111000000111111100010000000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LHU) {
    const auto instance = buildInstr(0b01111, 0b01100, 0b101000000111, LHU);
    Instruction parsed = decoder.parse(0b10100000011101100101011110000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, ADDI) {
    const auto instance = buildInstr(0b10111, 0b01000, 0b100011100101, ADDI);
    Instruction parsed = decoder.parse(0b10001110010101000000101110010011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, SLTI) {
    const auto instance = buildInstr(0b00111, 0b00001, 0b010101010111, SLTI);
    Instruction parsed = decoder.parse(0b01010101011100001010001110010011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, SLTIU) {
    const auto instance = buildInstr(0b11100, 0b00101, 0b000010100011, SLTIU);
    Instruction parsed = decoder.parse(0b00001010001100101011111000010011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, XORI) {
    const auto instance = buildInstr(0b00101, 0b00011, 0b001011100000, XORI);
    Instruction parsed = decoder.parse(0b00101110000000011100001010010011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, ORI) {
    const auto instance = buildInstr(0b10001, 0b01111, 0b100110101111, ORI);
    Instruction parsed = decoder.parse(0b10011010111101111110100010010011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, ANDI) {
    const auto instance = buildInstr(0b01011, 0b01101, 0b100001010111, ANDI);
    Instruction parsed = decoder.parse(0b10000101011101101111010110010011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, ECALL) {
    const auto instance = buildInstr(0b0, 0b0, 0b0, ECALL);
    Instruction parsed = decoder.parse(0b1110011);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, EBREAK) {
    const auto instance = buildInstr(0b0, 0b0, 0b1, EBREAK);
    Instruction parsed = decoder.parse(0b100000000000001110011);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, SRET) {
    const auto instance = buildInstr(0b0, 0b0, 0b100000010, SRET);
    Instruction parsed = decoder.parse(0b00010000001000000000000001110011);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, MRET) {
    const auto instance = buildInstr(0b0, 0b0, 0b1100000010, MRET);
    Instruction parsed = decoder.parse(0b00110000001000000000000001110011);
    EXPECT_EQ(parsed.operation, instance.operation);
    EXPECT_EQ(parsed.immidiate, instance.immidiate);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, NOT_EACALL_AND_EBREAK) {
    const auto instance = buildInstr(0b0, 0b0, 0b1, EBREAK);
    Instruction parsed = decoder.parse(0b100001000000001110011);
    EXPECT_FALSE(equal(parsed, instance));
}

TEST_F(Decoder_I, NOT_SRET_AND_MRET) {
    Instruction parsed = decoder.parse(0b01110000001000000000000001110011);
    EXPECT_EQ(parsed.operation, INV_OP);
}

TEST_F(Decoder_I, LWU1) {
    const auto instance = buildInstr(0b00001, 0b10111, 0b010110110110, LWU);
    Instruction parsed = decoder.parse(0b01011011011010111110000010000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LWU2) {
    const auto instance = buildInstr(0b11011, 0b01001, 0b000010001111, LWU);
    Instruction parsed = decoder.parse(0b00001000111101001110110110000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LD1) {
    const auto instance = buildInstr(0b00011, 0b10100, 0b100110101011, LD);
    Instruction parsed = decoder.parse(0b10011010101110100011000110000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, LD2) {
    const auto instance = buildInstr(0b10101, 0b10110, 0b000010010111, LD);
    Instruction parsed = decoder.parse(0b00001001011110110011101010000011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, NOT_LD) {
    const auto instance = buildInstr(0b11011, 0b10010, 0b100110010001, LD);
    Instruction parsed = decoder.parse(0b10011001000110010001110110000011);
    EXPECT_FALSE(equal(parsed, instance));
}

TEST_F(Decoder_I, ADDIW) {
    const auto instance = buildInstr(0b10110, 0b00101, 0b101101000001, ADDIW);
    Instruction parsed = decoder.parse(0b10110100000100101000101100011011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, SLLIW) {
    const auto instance = buildInstr(0b11110, 0b01010, 0b000000010010, SLLIW);
    Instruction parsed = decoder.parse(0b00000001001001010001111100011011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, SRLIW) {
    const auto instance = buildInstr(0b10110, 0b11010, 0b000000001001, SRLIW);
    Instruction parsed = decoder.parse(0b00000000100111010101101100011011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, SRAIW) {
    const auto instance = buildInstr(0b11001, 0b00100, 0b010000001000, SRAIW);
    Instruction parsed = decoder.parse(0b01000000100000100101110010011011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, NOT_SRLIW_OR_SRAIW) {
    Instruction parsed = decoder.parse(0b01100000100000100101110010011011);
    ASSERT_EQ(parsed.operation, INV_OP);
}

TEST_F(Decoder_I, CSRRW) {
    const auto instance = buildInstr(0b01010, 0b10111, 0b001000111100, CSRRW);
    Instruction parsed = decoder.parse(0b00100011110010111001010101110011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, CSRRS) {
    const auto instance = buildInstr(0b01101, 0b00100, 0b111110011100, CSRRS);
    Instruction parsed = decoder.parse(0b11111001110000100010011011110011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, CSRRC) {
    const auto instance = buildInstr(0b01011, 0b00110, 0b011001010001, CSRRC);
    Instruction parsed = decoder.parse(0b01100101000100110011010111110011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, CSRRWI) {
    const auto instance = buildInstr(0b10101, 0b01110, 0b101110111100, CSRRWI);
    Instruction parsed = decoder.parse(0b10111011110001110101101011110011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, CSRRSI) {
    const auto instance = buildInstr(0b11010, 0b10001, 0b110000010010, CSRRSI);
    Instruction parsed = decoder.parse(0b11000001001010001110110101110011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_I, CSRRCI) {
    const auto instance = buildInstr(0b00011, 0b11001, 0b101111001011, CSRRCI);
    Instruction parsed = decoder.parse(0b10111100101111001111000111110011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SD1) {
    const auto instance = buildInstr(0b00100, 0b00100, 0b111111111111, SD);
    Instruction parsed = decoder.parse(0b11111110010000100011111110100011);
    ASSERT_EQ(parsed.operation, SD);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SD2) {
    const auto instance = buildInstr(0b00100, 0b00100, 0b000000000001, SD);
    Instruction parsed = decoder.parse(0b00000000010000100011000010100011);
    ASSERT_EQ(parsed.operation, SD);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SD3) {
    const auto instance = buildInstr(0b11011, 0b11011, 0b000000010000, SD);
    Instruction parsed = decoder.parse(0b00000001101111011011100000100011);
    ASSERT_EQ(parsed.operation, SD);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SD4) {
    const auto instance = buildInstr(0b11011, 0b11011, 0b000000100000, SD);
    Instruction parsed = decoder.parse(0b00000011101111011011000000100011);
    ASSERT_EQ(parsed.operation, SD);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SD5) {
    const auto instance = buildInstr(0b11011, 0b11011, 0b100000000000, SD);
    Instruction parsed = decoder.parse(0b10000001101111011011000000100011);
    ASSERT_EQ(parsed.operation, SD);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SB) {
    const auto instance = buildInstr(0b01000, 0b11110, 0b101000111010, SB);
    Instruction parsed = decoder.parse(0b10100011111001000000110100100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SH) {
    const auto instance = buildInstr(0b11011, 0b10010, 0b010101111010, SH);
    Instruction parsed = decoder.parse(0b01010111001011011001110100100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_S, SW) {
    const auto instance = buildInstr(0b11010, 0b10100, 0b110101111010, SW);
    Instruction parsed = decoder.parse(0b11010111010011010010110100100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BEQ1) {
    const auto instance = buildInstr(0b11011, 0b11011, 0b1111111111110, BEQ);
    Instruction parsed = decoder.parse(0b11111111101111011000111111100011);
    ASSERT_EQ(parsed.operation, BEQ);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BEQ2) {
    const auto instance = buildInstr(0b11011, 0b11011, 0b0000000000000, BEQ);
    Instruction parsed = decoder.parse(0b00000001101111011000000001100011);
    ASSERT_EQ(parsed.operation, BEQ);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BEQ3) {
    const auto instance = buildInstr(0b01001, 0b11101, 0b0111001110010, BEQ);
    Instruction parsed = decoder.parse(0b01100111110101001000100111100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BNE) {
    const auto instance = buildInstr(0b01001, 0b11101, 0b0111001110010, BNE);
    Instruction parsed = decoder.parse(0b01100111110101001001100111100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BLT) {
    const auto instance = buildInstr(0b00101, 0b11011, 0b1101010011100, BLT);
    Instruction parsed = decoder.parse(0b10101001101100101100111011100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BGE) {
    const auto instance = buildInstr(0b00110, 0b00000, 0b0100000001110, BGE);
    Instruction parsed = decoder.parse(0b00000000000000110101011111100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BLTU) {
    const auto instance = buildInstr(0b00101, 0b10110, 0b0111100101010, BLTU);
    Instruction parsed = decoder.parse(0b01110011011000101110010111100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, BGEU) {
    const auto instance = buildInstr(0b01100, 0b00000, 0b0111101001110, BGEU);
    Instruction parsed = decoder.parse(0b01110100000001100111011111100011);
    EXPECT_TRUE(equal(parsed, instance));
}

TEST_F(Decoder_B, NOT_BEQ) {
    // this is the test with opcode 0b1100011 but non-existent func3 = 0b010
    Instruction parsed = decoder.parse(0b01110100000001100010011111100011);
    EXPECT_EQ(parsed.operation, INV_OP);
}
