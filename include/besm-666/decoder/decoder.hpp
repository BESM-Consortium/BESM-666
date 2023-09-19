#pragma once
#include "besm-666/autogen/operations-matrix.hpp"
#include "besm-666/instruction.hpp"
#include <cassert>
#include <cstdlib>
#include <stdexcept>

namespace besm::dec {

class Decoder {

private:
    static constexpr RV64UWord OPCODE_MASK = 0b01111111;
    static constexpr int FUNC3_shift = 12;
    static constexpr RV64UWord FUNC3_MASK = 0b111 << FUNC3_shift;
    static constexpr int RD_SHIFT = 7;
    static constexpr RV64UWord RD_MASK = 0b11111 << RD_SHIFT;
    static constexpr int RS1_SHIFT = 15;
    static constexpr RV64UWord RS1_MASK = 0b11111 << RS1_SHIFT;
    static constexpr int RS2_SHIFT = 20;
    static constexpr RV64UWord RS2_MASK = 0b11111 << RS2_SHIFT;

public:
    /**
     * Give the {@link Instruction} by the word.
     * @param word word.
     * @return {@link Instruction} struct.
     */
    besm::Instruction parse(const RV64UWord word) const {
        const Opcode opcode = word & OPCODE_MASK;
        const uint8_t func3 = (word & FUNC3_MASK) >> FUNC3_shift;
        assert(opcode < 128);
        assert(func3 < 8);
        const Cell *cell = &(SUPER_MEGA_MATRIX[opcode][func3]);
        const Format format = cell->format;
        const InstructionOp operation = cell->operation;
        switch (format) {
        case R:
            return parse_R(word, opcode, func3);
            break;
        case I:
            return parse_I(word, operation, opcode);
            break;
        case S:
            return parse_S(word, operation);
            break;
        case B:
            return parse_B(word, operation);
            break;
        case U:
            return parse_U(word, operation);
            break;
        case J:
            return parse_J(word, operation);
            break;
        }
        return Instruction{.operation = NON_OP};
    }

private:
    static inline Instruction parse_R(const RV64UWord word, const Opcode opcode,
                                      const uint8_t func3) {
        constexpr int FUNC7_SHIFT = 25;
        constexpr RV64UWord FUNC7_MASK = 0b1111111 << FUNC7_SHIFT;
        const uint16_t func7 = (word & FUNC7_MASK) >> (FUNC7_SHIFT - 3);
        const uint16_t func10 = func7 | func3;
        assert(func10 < 0b10000000000);
        InstructionOp operation = NON_OP;
        switch (opcode) {
        case 0b0110011:
            switch (func10) {
            case 0b0000000000:
                operation = ADD;
                break;
            case 0b0100000000:
                operation = SUB;
                break;
            case 0b0000000001:
                operation = SLL;
                break;
            case 0b0000000010:
                operation = SLT;
                break;
            case 0b0000000011:
                operation = SLTU;
                break;
            case 0b0000000100:
                operation = XOR;
                break;
            case 0b0000000101:
                operation = SRL;
                break;
            case 0b0100000101:
                operation = SRA;
                break;
            case 0b0000000110:
                operation = OR;
                break;
            case 0b0000000111:
                operation = AND;
            }
            break;
        case 0b0111011:
            switch (func10) {
            case 0b0000000000:
                operation = ADDW;
                break;
            case 0b0100000000:
                operation = SUBW;
                break;
            case 0b0000000001:
                operation = SLLW;
                break;
            case 0b0000000101:
                operation = SRLW;
                break;
            case 0b0100000101:
                operation = SRAW;
                break;
            }
            break;
        }
        return Instruction{
            .rd = static_cast<Register>((word & RD_MASK) >> RD_SHIFT),
            .rs1 = static_cast<Register>((word & RS1_MASK) >> RS1_SHIFT),
            .rs2 = static_cast<Register>((word & RS2_MASK) >> RS2_SHIFT),
            .immidiate = IMMIDIATE_POISON,
            .operation = operation};
    }

    /*
     * @todo #1ILLEGAL:90m Implement the functions of I format. Now it is
     * just a stub. We need to implement every of them and test.
     * ALARM: pay attention to 0b1110011 opcode.
     */
    static inline Instruction parse_I(const RV64UWord word,
                                      const InstructionOp operation,
                                      const Opcode opcode) {
        return Instruction{};
    }

    /*
     * @todo #1ILLEGAL:9ILLEGALm Implement the functions of S format. Now it is
     * just a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_S(const RV64UWord word,
                                      const InstructionOp operation) {
        return Instruction{};
    }

    /*
     * @todo #1ILLEGAL:9ILLEGALm Implement the functions of B format. Now it is
     * just a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_B(const RV64UWord word,
                                      const InstructionOp operation) {
        return Instruction{};
    }

    /*
     * @todo #1ILLEGAL:9ILLEGALm Implement the functions of U format. Now it is
     * just a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_U(const RV64UWord word,
                                      const InstructionOp operation) {
        return Instruction{};
    }

    /*
     * @todo #1ILLEGAL:9ILLEGALm Implement the functions of J format. Now it is
     * just a stub. We need to implement every of them and test.
     */
    static inline Instruction parse_J(const RV64UWord word,
                                      const InstructionOp operation) {
        return Instruction{};
    }
};

} // namespace besm::dec
