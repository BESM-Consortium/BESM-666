#ifndef BESM666_RISCV_INSTRUCTION_H
#define BESM666_RISCV_INSTRUCTION_H

#include "besm-666/riscv-types.hpp"

namespace besm {

using Register = uint8_t;
using Opcode = uint8_t;
using Immidiate = uint64_t;

enum class OperandType { UNUSED, REGISTER, IMMIDIATE };

struct Operand {
    OperandType type;
    union {
        Register reg;
        Immidiate imm;
    };
};

struct Instruction {
    Opcode opcode;
    Operand op1;
    Operand op2;
    Operand op3;
};

} // namespace besm

#endif /* BESM666_RISCV_INSTRUCTION_H */
