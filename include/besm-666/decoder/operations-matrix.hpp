#pragma once
#include "besm-666/rv-instruction-op.hpp"
#include "opcodes-to-format.hpp"

namespace besm::dec {

struct Cell {
    const format_t format;
    const InstructionOp operation;
};

constexpr static Cell SUPER_MEGA_MATRIX[128][8] = {
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b0
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10
    {
        {I, LB},
        {I, LH},
        {I, LW},
        {I, LD},
        {I, LBU},
        {I, LHU},
        {I, LWU},
        {I, NON_OP},
    }, // opcode: 0b11
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110
    {
        {I, PAUSE},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
    }, // opcode: 0b1111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10010
    {
        {I, ADDI},
        {I, SLLI},
        {I, SLTI},
        {I, SLTIU},
        {I, XORI},
        {I, SRAI},
        {I, ORI},
        {I, ANDI},
    }, // opcode: 0b10011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b10110
    {
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
    }, // opcode: 0b10111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11010
    {
        {I, ADDIW},
        {I, SLLIW},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, SRAIW},
        {I, NON_OP},
        {I, NON_OP},
    }, // opcode: 0b11011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b11111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100010
    {
        {S, SB},
        {S, SH},
        {S, SW},
        {S, SD},
        {S, NON_OP},
        {S, NON_OP},
        {S, NON_OP},
        {S, NON_OP},
    }, // opcode: 0b100011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b100111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b101111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110010
    {
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
    }, // opcode: 0b110011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b110110
    {
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
        {U, NON_OP},
    }, // opcode: 0b110111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111010
    {
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
        {R, NON_OP},
    }, // opcode: 0b111011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b111111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1000111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1001111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1010111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1011111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100010
    {
        {B, BEQ},
        {B, BNE},
        {B, NON_OP},
        {B, NON_OP},
        {B, BLT},
        {B, BGE},
        {B, BLTU},
        {B, BGEU},
    }, // opcode: 0b1100011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1100110
    {
        {I, JALR},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
    }, // opcode: 0b1100111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1101110
    {
        {J, NON_OP},
        {J, NON_OP},
        {J, NON_OP},
        {J, NON_OP},
        {J, NON_OP},
        {J, NON_OP},
        {J, NON_OP},
        {J, NON_OP},
    }, // opcode: 0b1101111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110010
    {
        {I, EBREAK},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
        {I, NON_OP},
    }, // opcode: 0b1110011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1110111
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111000
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111001
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111010
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111011
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111100
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111101
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111110
    {
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
        {ILLEGAL, NON_OP},
    }, // opcode: 0b1111111
};
} // namespace besm::dec
