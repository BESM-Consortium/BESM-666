#pragma once

#include "besm-666/riscv-types.hpp"
#include "besm-666/rv-instruction-op.hpp"

namespace besm {

using Register = uint8_t;
using Opcode = uint8_t;
using Immidiate = uint32_t;

struct Instruction {
    const Register rd;
    const Register rs1;
    const Register rs2;
    const Immidiate immidiate;
    const InstructionOp operation;

    static constexpr Immidiate IMMIDIATE_POISON = UINT32_MAX;
};

} // namespace besm
