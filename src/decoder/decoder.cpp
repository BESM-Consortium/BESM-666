#include "besm-666/decoder/decoder.hpp"
#include "besm-666/util/bit-magic.hpp"

using namespace besm;

Instruction dec::Decoder::parse(const RV64UWord bytecode) const {
    const Opcode opcode = bytecode & OPCODE_MASK;
    const uint8_t func3 = (bytecode & FUNC3_MASK) >> FUNC3_shift;
    assert(opcode < 128);
    assert(func3 < 8);
    const Cell *cell = &(INSTR_WHO[opcode][func3]);
    const Format format = cell->format;
    const InstructionOp operation = cell->operation;
    printf("opcode = %i, operation = %i\n", opcode, operation);
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
    return Instruction{.operation = NON_OP};
}

Instruction dec::Decoder::parse_R(const RV64UWord bytecode, const Opcode opcode,
                                  const uint8_t func3) {
    constexpr int FUNC7_SHIFT = 25;
    constexpr RV64UWord FUNC7_MASK = 0b1111111 << FUNC7_SHIFT;
    const uint16_t func7 = (bytecode & FUNC7_MASK) >> (FUNC7_SHIFT - 3);
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
        .rd = static_cast<Register>((bytecode & RD_MASK) >> RD_SHIFT),
        .rs1 = static_cast<Register>((bytecode & RS1_MASK) >> RS1_SHIFT),
        .rs2 = static_cast<Register>((bytecode & RS2_MASK) >> RS2_SHIFT),
        .immidiate = Instruction::IMMIDIATE_POISON,
        .operation = operation};
}

inline Instruction dec::Decoder::parse_U(const RV64UWord bytecode,
                                         const InstructionOp operation) {
    constexpr int IMM_SHIFT = 12;
    constexpr RV64UWord IMM_MASK = 0b11111111111111111111 << IMM_SHIFT;
    return Instruction{
        .rd = static_cast<Register>((bytecode & RD_MASK) >> RD_SHIFT),
        .immidiate = ((bytecode & IMM_MASK) >> IMM_SHIFT),
        .operation = operation};
}
Instruction dec::Decoder::parse_I(const RV64UWord bytecode,
                                  const InstructionOp operation,
                                  const Opcode opcode) {
    return Instruction{};
}
Instruction dec::Decoder::parse_S(RV64UWord bytecode, InstructionOp operation) {
    return Instruction{};
}
Instruction dec::Decoder::parse_B(const RV64UWord bytecode,
                                  const InstructionOp operation) {
    return Instruction{};
}
Instruction dec::Decoder::parse_J(const RV64UWord bytecode,
                                  const InstructionOp operation) {
    // format J: imm[20, 10:11, 11, 19:12] [rd(5 bits)] [opcode(7 bits)]
    const auto bit20 = util::ExtractBits<RV64UWord, 1, 12 + 19>(bytecode) << 19;
    const auto bit1_10 = util::ExtractBits<RV64UWord, 10, 12 + 9>(bytecode) << 0;
    const auto bit11 = util::ExtractBits<RV64UWord, 1, 12 + 8>(bytecode) << 10;
    const auto bit12_19 = util::ExtractBits<RV64UWord, 8, 12 + 0>(bytecode) << 11;
    printf("%u, %u, %u, %u\n", bit20, bit1_10, bit11, bit12_19);
    printf("unite = %i\n", bit20 & bit1_10 & bit11 & bit12_19 );
    //assert(bit20 & bit1_10 & bit11 & bit12_19 == (RV64UWord)0b0);
    return Instruction{
        .rd = static_cast<Register>((bytecode & RD_MASK) >> RD_SHIFT),
        .immidiate = bit20 | bit1_10 | bit11 | bit12_19,
        .operation = operation};
}
