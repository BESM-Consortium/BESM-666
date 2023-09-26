#pragma once
#include "besm-666/decoder/decoder.hpp"
#include "besm-666/rv-instruction-op.hpp"

namespace besm::dec {

/*
 * @todo #10:90m Generate this file instead of copypaste from the current raw
 * generator.
 */

enum Format { ILLEGAL, R, I, S, B, U, J };

struct Cell {
    const Format format;
    const InstructionOp operation;
};

constexpr static Cell INSTR_WHO[128][8] = {
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b0
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b10
    {
        {I, LB},
        {I, LH},
        {I, LW},
        {I, LD},
        {I, LBU},
        {I, LHU},
        {I, LWU},
        {I, INV_OP},
    }, // opcode: 0b11
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110
    {
        {I, PAUSE},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
    }, // opcode: 0b1111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b10000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b10001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
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
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b10100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b10101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b10110
    {
        {U, AUIPC},
        {U, AUIPC},
        {U, AUIPC},
        {U, AUIPC},
        {U, AUIPC},
        {U, AUIPC},
        {U, AUIPC},
        {U, AUIPC},
    }, // opcode: 0b10111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11010
    {
        {I, ADDIW},
        {I, SLLIW},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
    }, // opcode: 0b11011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b11111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100010
    {
        {S, SB},
        {S, SH},
        {S, SW},
        {S, SD},
        {S, INV_OP},
        {S, INV_OP},
        {S, INV_OP},
        {S, INV_OP},
    }, // opcode: 0b100011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b100111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b101111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110010
    {
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
    }, // opcode: 0b110011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b110110
    {
        {U, LUI},
        {U, LUI},
        {U, LUI},
        {U, LUI},
        {U, LUI},
        {U, LUI},
        {U, LUI},
        {U, LUI},
    }, // opcode: 0b110111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111010
    {
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
        {R, INV_OP},
    }, // opcode: 0b111011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b111111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1000111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1001111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1010111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1011111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100010
    {
        {B, BEQ},
        {B, BNE},
        {B, INV_OP},
        {B, INV_OP},
        {B, BLT},
        {B, BGE},
        {B, BLTU},
        {B, BGEU},
    }, // opcode: 0b1100011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1100110
    {
        {I, JALR},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
    }, // opcode: 0b1100111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1101110
    {
        {J, JAL},
        {J, JAL},
        {J, JAL},
        {J, JAL},
        {J, JAL},
        {J, JAL},
        {J, JAL},
        {J, JAL},
    }, // opcode: 0b1101111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110010
    {
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
        {I, INV_OP},
    }, // opcode: 0b1110011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1110111
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111000
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111001
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111010
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111011
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111100
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111101
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111110
    {
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
        {ILLEGAL, INV_OP},
    }, // opcode: 0b1111111
};

} // namespace besm::dec
