#include "besm-666/decoder/decoder.hpp"
#include "besm-666/util/bit-magic.hpp"
#include <iostream>

using namespace besm;

static constexpr int REG_WIDTH = 5;
static constexpr int OPCODE_WIDTH = 7;
static constexpr int FUNC3_WIDTH = 3;
using util::ExtractBits;

template <size_t From>
static inline Register ExtractRegister(RV64UWord bytecode) {
    return ExtractBits<RV64UWord, Register, REG_WIDTH, From>(bytecode);
}

Instruction dec::Decoder::parse(const RV64UWord bytecode) const {
    const Opcode opcode = ExtractBits<RV64UWord, OPCODE_WIDTH>(bytecode);
    const uint8_t func3 =
        ExtractBits<RV64UWord, FUNC3_WIDTH, FUNC3_SHIFT>(bytecode);
    assert(opcode < 128);
    assert(func3 < 8);
    const Cell *cell = &(INSTR_WHO[opcode][func3]);
    const Format format = cell->format;
    const InstructionOp operation = cell->operation;
    switch (format) {
    case R:
        return parse_R(bytecode, opcode, func3);
        break;
    case I:
        return parse_I(bytecode, operation, opcode);
        break;
    case S:
        return parse_S(bytecode, operation);
        break;
    case B:
        return parse_B(bytecode, operation);
        break;
    case U:
        return parse_U(bytecode, operation);
        break;
    case J:
        return parse_J(bytecode, operation);
        break;
    }
    return Instruction{.operation = INV_OP};
}

Instruction dec::Decoder::parse_R(const RV64UWord bytecode, const Opcode opcode,
                                  const uint8_t func3) {
    constexpr int FUNC7_SHIFT = 25;
    constexpr RV64UWord FUNC7_MASK = 0b1111111 << FUNC7_SHIFT;
    constexpr Opcode OP = 0b0110011;
    constexpr Opcode OP32 = 0b0111011;
    const uint16_t func7 = (bytecode & FUNC7_MASK) >> (FUNC7_SHIFT - 3);
    const uint16_t func10 = func7 | func3;
    assert(func10 < 0b10000000000);
    InstructionOp operation = INV_OP;
    switch (opcode) {
    case OP:
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
    case OP32:
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
    return Instruction{.rd = ExtractRegister<RD_SHIFT>(bytecode),
                       .rs1 = ExtractRegister<RS1_SHIFT>(bytecode),
                       .rs2 = ExtractRegister<RS2_SHIFT>(bytecode),
                       .immidiate = Instruction::IMMIDIATE_POISON,
                       .operation = operation};
}

inline Instruction dec::Decoder::parse_U(const RV64UWord bytecode,
                                         const InstructionOp operation) {
    constexpr int IMM_SHIFT = 12;
    constexpr RV64UWord IMM_MASK = 0b11111111111111111111 << IMM_SHIFT;
    return Instruction{.rd = ExtractRegister<RD_SHIFT>(bytecode),
                       .immidiate = (bytecode & IMM_MASK),
                       .operation = operation};
}
Instruction dec::Decoder::parse_I(const RV64UWord bytecode,
                                  InstructionOp operation,
                                  const Opcode opcode) {
    const auto imm = util::ExtractBits<Immidiate, 12, 20>(bytecode);
    const auto rd = static_cast<Register>((bytecode & RD_MASK) >> RD_SHIFT);
    const auto rs1 = ExtractRegister<RS1_SHIFT>(bytecode);
    if (operation != INV_OP) {
        return Instruction{
            .rd = rd, .rs1 = rs1, .immidiate = imm, .operation = operation};
    }
    const uint8_t func3 =
        util::ExtractBits<RV64UWord, 3, FUNC3_SHIFT>(bytecode);
    constexpr Opcode MISC_MEM = 0b0001111;
    constexpr Opcode SYSTEM = 0b1110011;
    constexpr Opcode OPP_IMM32 = 0b0011011;
    switch (opcode) {
    case MISC_MEM:
        /*
         * @todo #39:90min Implement processing of 0b0001111 opcode.
         *  According to the new specification
         *  https://raw.githubusercontent.com/riscv/riscv-isa-manual/8176af1a46bfe2e15f8cdb170d74e52e1ba6efa2/src/rv-32-64g.adoc
         *  there are not FENCE only but also FENCE.TSO and PAUSE with
         * non-trivial decoding. Nor it just returns FENCE operation.
         */
        return Instruction{
            .rd = rd, .rs1 = rs1, .immidiate = imm, .operation = FENCE};
        break;
    case SYSTEM:
        if (rd == 0b0 && rs1 == 0b0 && func3 == 0b0) {
            switch (imm) {
            case 0b0:
                operation = ECALL;
                break;
            case 0b1:
                operation = EBREAK;
                break;
            case 0b000100000010:
                operation = SRET;
                break;
            case 0b001100000010:
                operation = MRET;
                break;
            }
        }
        break;
    case OPP_IMM32:
        switch (bytecode >> 25) {
        case 0b0:
            operation = SRLIW;
            break;
        case 0b0100000:
            operation = SRAIW;
            break;
        }
        break;
    }
    return Instruction{
        .rd = rd, .rs1 = rs1, .immidiate = imm, .operation = operation};
}
Instruction dec::Decoder::parse_S(RV64UWord bytecode, InstructionOp operation) {
    // [imm 11:0][rs1][func3][rd][opcode]
    const auto imm11_5 = util::ExtractBits<Immidiate, 7, 25>(bytecode) << 5;
    const auto imm4_0 = util::ExtractBits<Immidiate, 5, 7>(bytecode) << 0;

    assert((imm4_0 & imm11_5) == (RV64UWord)0);
    return Instruction{
        .rs1 = ExtractRegister<RS1_SHIFT>(bytecode),
        .rs2 = ExtractRegister<RS2_SHIFT>(bytecode),
        .immidiate = imm4_0 | imm11_5,
        .operation = operation,
    };
}
Instruction dec::Decoder::parse_B(const RV64UWord bytecode,
                                  const InstructionOp operation) {
    const auto imm12 = util::ExtractBits<Immidiate, 1, 31>(bytecode) << 12;
    const auto imm5_10 = util::ExtractBits<Immidiate, 6, 25>(bytecode) << 5;
    const auto imm1_4 = util::ExtractBits<Immidiate, 4, 8>(bytecode) << 1;
    const auto imm11 = util::ExtractBits<Immidiate, 1, 7>(bytecode) << 11;
    assert((imm1_4 & imm5_10 & imm11 & imm12) == (RV64UWord)0);
    return Instruction{.rs1 = ExtractRegister<RS1_SHIFT>(bytecode),
                       .rs2 = ExtractRegister<RS2_SHIFT>(bytecode),
                       .immidiate = imm1_4 | imm5_10 | imm11 | imm12,
                       .operation = operation};
}

Instruction dec::Decoder::parse_J(const RV64UWord bytecode,
                                  const InstructionOp operation) {
    // format J: imm[20, 10:11, 11, 19:12] [rd(5 bits)] [opcode(7 bits)]
    const auto bit20 = util::ExtractBits<RV64UWord, 1, 12 + 19>(bytecode) << 20;
    const auto bit1_10 = util::ExtractBits<RV64UWord, 10, 12 + 9>(bytecode)
                         << 1;
    const auto bit11 = util::ExtractBits<RV64UWord, 1, 12 + 8>(bytecode) << 11;
    const auto bit12_19 = util::ExtractBits<RV64UWord, 8, 12 + 0>(bytecode)
                          << 12;
    assert((bit20 & bit1_10 & bit11 & bit12_19) == 0b0);
    return Instruction{.rd = ExtractRegister<RD_SHIFT>(bytecode),
                       .immidiate = bit20 | bit1_10 | bit11 | bit12_19,
                       .operation = operation};
}
