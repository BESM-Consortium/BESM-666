#pragma once

#include "besm-666/riscv-types.hpp"
#include "besm-666/rv-instruction-op.hpp"

namespace besm {

using Opcode = uint8_t;
using Register = uint8_t;
using Immidiate = uint32_t;

struct Instruction {
    Register rd;
    Register rs1;
    Register rs2;
    Immidiate immidiate;
    InstructionOp operation;
};

} // namespace besm
